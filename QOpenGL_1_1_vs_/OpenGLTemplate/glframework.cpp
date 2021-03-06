/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "glframework.h"

#include <limits>
#include <QMouseEvent>
#include <QtMath>

YGL_USE_NAMESPACE
const int bottom[] = {42,43,50,11,8,856,19,16,839,840,860,51,48,49,56,57,823,
83,84,22,23,1015,1073,1074,1741,1047,1048,1041,1040,1043,1778,1758,1757,1011,
1012,1774,1003,1004,1042,1035,1036,1086,1085,0,1,98,96};
GLFramework::GLFramework(QWidget *parent)
    : QOpenGLWidget(parent)
    , mEnabledGrid(false)
    , mEnabledLocator(false)
{
    mTriMesh = NULL;
    
    mCacheRotPos = QVector2D(-1,-1); // -1 as NULL
}

GLFramework::~GLFramework()
{
}

bool GLFramework::LoadMesh(OMT::MyTriMesh * mesh, std::string filename){
	bool isRead = false;
	OpenMesh::IO::Options opt;
	if (OpenMesh::IO::read_mesh(*mesh, filename, opt)){
		//read mesh form filename ok
		isRead = true;
	}
	if (isRead){
		//if the file did not provide vertex normals
		//and mesh has vertex normal, then calculate them
		if (!opt.check(OpenMesh::IO::Options::VertexNormal) && mesh->has_vertex_normals()){
			mesh->update_normals();
		}
	}
	return isRead;
}

QOpenGLTexture * GLFramework::LoadGLTextures(const char * name){
	QImage img;
	bool flag = img.load(name);
	if (flag){
		QOpenGLTexture * texture = new QOpenGLTexture(img.mirrored());
		texture->setMinificationFilter(QOpenGLTexture::Linear);
		texture->setMagnificationFilter(QOpenGLTexture::Linear);
		texture->setWrapMode(QOpenGLTexture::ClampToEdge);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		return texture;
	}else{
		std::cout << "Load \"" << name << "\" Failed" << std::endl;
		return NULL;
	}
}

void GLFramework::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mCacheRotPos = QVector2D(e->localPos());
    this->update();
}

void GLFramework::mouseReleaseEvent(QMouseEvent *e)
{

}

void GLFramework::mouseMoveEvent(QMouseEvent *e)
{

    // Rotation phase
    QVector2D diff = QVector2D(e->localPos()) - mCacheRotPos;
    mCacheRotPos = QVector2D(e->localPos());
    mTrackBallCamera.rotate(diff / qMax(width(), height()) * 180.0f);

	float x = e->localPos().x();
	float y = e->localPos().y();
	//cout << "x: " << x << " y: " << y << std::endl;
	gl_x = x; gl_y = 635-y;

    this->update();
}

void GLFramework::wheelEvent(QWheelEvent *event)
{
    qreal delta = event->angleDelta().x() + event->angleDelta().y();

    mTrackBallCamera.zoom( - delta / 360.0f );

    this->update();
}

void GLFramework::timerEvent(QTimerEvent *)
{
     //this->update();
}

void GLFramework::initializeGL()
{
    /* Important OpenGL initialization */
    initializeOpenGLFunctions();

    /* Initialize background color to white */
    glClearColor(1, 1, 1, 1);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    
  //  glDepthMask(GL_TRUE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//====================================
	/* texture loading */
	floor = LoadGLTextures("floor.bmp");
	wall1 = LoadGLTextures("wall1.bmp");
	wall2 = LoadGLTextures("wall2.bmp");
	wall3 = LoadGLTextures("wall3.bmp");

//====================================
    /* Call every 16ms */
    mTimer.start(16, this);
}

void GLFramework::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 300.0, field of view 45 degrees
    const qreal zNear = 0.1, zFar = 2000.0, fov = 45.0;

    mTrackBallCamera.setPerspective(fov, aspect, zNear, zFar);
    // trackBallCamera.setOrthogonal(0, w, 0, h, zNear, zFar);
}

void GLFramework::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

    applyCameraSetting( mTrackBallCamera );

	drawScene();

    glDisable(GL_BLEND);

    // If mesh exist, render it
    if (mTriMesh)
    {
        RenderSolid();
    }

    if ( mEnabledLocator ) drawLocator(3);
    if ( mEnabledGrid ) drawGrid(20);
	meshVertexRender(gl_x, gl_y);
	if (mTriMesh)	{ 
		drawCurrentFace(); 
		drawStoredFace();
		drawSelectedVertex();
	}
	if (sortedBorder.size() != 0)
		drawBorder();
}

void GLFramework::applyCameraSetting(Camera &cam)
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(cam.getProjectMatrix().data());

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(cam.transform.getWorldToObjectMatrix().data());
}

void GLFramework::drawGrid(int size)
{
    glLineWidth(1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);

    glBegin(GL_LINES);
    for(int i = -size; i <= size; i++)
    {
        glVertex3f(    i, 0, size);
        glVertex3f(    i, 0,-size);
        glVertex3f( size, 0,    i);
        glVertex3f(-size, 0,    i);
    }
    glEnd();
}

void GLFramework::drawLocator(float size)
{
    glLineWidth(3.0f);

    glBegin(GL_LINES);
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0,0,0);
        glVertex3f(size,0,0);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0,0,0);
        glVertex3f(0,size,0);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0,0,0);
        glVertex3f(0,0,size);
    }
    glEnd();
}

void GLFramework::RenderSolid()
{
    // Draw mesh faces
    OMT::FIter f_it;
    OMT::FVIter fv_it;

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.0, 2.0);
    glBegin(GL_TRIANGLES);
    glColor4f(0.81f, 0.74f, 0.33f, 1.0f);
    for (f_it = mTriMesh->faces_begin(); f_it != mTriMesh->faces_end(); ++f_it)
    {
        for (fv_it = mTriMesh->fv_iter(f_it); fv_it; ++fv_it)
        {
           glNormal3dv(mTriMesh->normal(fv_it.handle()).data());
           glVertex3dv(mTriMesh->point(fv_it.handle()).data());
        }
    }
    glEnd();

    glDisable(GL_POLYGON_OFFSET_FILL);
    // temporary add find nearest point of mesh here
	//OMT::VIter v_it;
	//for (v_it = mTriMesh->vertices_begin();
	//	v_it != mTriMesh->vertices_end();
	//	++v_it);
    // Draw mesh edges
    glLineWidth(1.0);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    for (OMT::EIter e_it = mTriMesh->edges_begin(); e_it != mTriMesh->edges_end(); ++e_it)
    {
        OMT::HEHandle _hedge = mTriMesh->halfedge_handle(e_it.handle(), 1);

        OMT::Point curVertex = mTriMesh->point(mTriMesh->from_vertex_handle(_hedge));
        glVertex3dv(curVertex.data());

        curVertex = mTriMesh->point(mTriMesh->to_vertex_handle(_hedge));
        glVertex3dv(curVertex.data());
    }
    glEnd();
}

void GLFramework::drawScene(){

	glEnable(GL_TEXTURE_2D);
//=============================
	/* draw floor */
	glPushMatrix();
	glRotatef(270,1,0,0);
	glScalef(20, 20, 1);
	glColor3f(1.0, 0.0, 0.0);
	floor->bind();
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2d(-1, -1);
	glTexCoord2d(1, 0); glVertex2d(1, -1);
	glTexCoord2d(1, 1); glVertex2d(1, 1);
	glTexCoord2d(0, 1); glVertex2d(-1, 1);
	glEnd();
	glPopMatrix();
//==============================
	/* draw wall 1 */
	glPushMatrix();
	glRotatef(270, 0, 1, 0);
	glScalef(20, 20, 1);
	glTranslatef(0.0, 1.0, -20.0);
	glColor3f(1.0, 0.0, 0.0);
	wall1->bind();
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2d(-1, -1);
	glTexCoord2d(1, 0); glVertex2d(1, -1);
	glTexCoord2d(1, 1); glVertex2d(1, 1);
	glTexCoord2d(0, 1); glVertex2d(-1, 1);
	glEnd();
	glPopMatrix();
//==============================
	/* draw wall 2 */
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glScalef(20, 20, 1);
	glTranslatef(0.0, 1.0, -20.0);
	glColor3f(1.0, 0.0, 0.0);
	wall2->bind();
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2d(-1, -1);
	glTexCoord2d(1, 0); glVertex2d(1, -1);
	glTexCoord2d(1, 1); glVertex2d(1, 1);
	glTexCoord2d(0, 1); glVertex2d(-1, 1);
	glEnd();
	glPopMatrix();
//================================
	/* draw wall 3 */
	glPushMatrix();
	//glRotatef(90, 0, 1, 0);
	glScalef(20, 20, 1);
	glTranslatef(0.0, 1.0, -20.0);
	glColor3f(1.0, 0.0, 0.0);
	wall3->bind();
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2d(-1, -1);
	glTexCoord2d(1, 0); glVertex2d(1, -1);
	glTexCoord2d(1, 1); glVertex2d(1, 1);
	glTexCoord2d(0, 1); glVertex2d(-1, 1);
	glEnd();
	glPopMatrix();
//=================================
	glDisable(GL_TEXTURE_2D);
}

void GLFramework::meshVertexRender(GLfloat x_coor, GLfloat y_coor){
	GLfloat depth=-1;
	unsigned char color[3];
	applyCameraSetting(mTrackBallCamera);
	glReadBuffer(GL_FRONT);
	glGetDoublev(GL_MODELVIEW_MATRIX, current_modelview_matrix);
	glGetDoublev(GL_PROJECTION_MATRIX, current_projection_matrix);
	glGetIntegerv(GL_VIEWPORT, current_viewport_matrix);
	glReadPixels(x_coor, y_coor, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	
	gluUnProject(x_coor,y_coor,depth,current_modelview_matrix,current_projection_matrix
		,current_viewport_matrix,&objX,&objY,&objZ);
	

	//if (mTriMesh){ findNearestVertex(objX, objY, objZ); }
	if (mTriMesh){ findNearestFace(); }
	
}

void GLFramework::findNearestVertex(GLfloat objx, GLfloat objy, GLfloat objz){
	OMT::VIter v_it;
	OMT::VIter aim;
	int flag = 0;
	
	float distance = 1000;
	//cout << "find nearest vertex function called";
	for (v_it = mTriMesh->vertices_begin();
		v_it != mTriMesh->vertices_end();
		++v_it){
		auto point = mTriMesh->point(v_it.handle());
		if (distance > getDistance(point.data()[0], point.data()[1], point.data()[2]))
		{
			distance = getDistance(point.data()[0], point.data()[1], point.data()[2]);
			aim = v_it;
			flag = 1;
		}
	}	
}

void GLFramework::findNearestFace(){
	float distance = 1000;
	//int PointedFaceIdx;
	for (OMT::FIter f_it = mTriMesh->faces_begin();
		f_it != mTriMesh->faces_end();
		f_it++){
		float midx = 0, midy = 0, midz = 0;
		//float sumx = 0, sumy, sumz;
		for (OMT::FVIter fv_it = mTriMesh->fv_iter(f_it); fv_it; ++fv_it){
			midx += mTriMesh->point(fv_it.handle()).data()[0];
			midy += mTriMesh->point(fv_it.handle()).data()[1];
			midz += mTriMesh->point(fv_it.handle()).data()[2];
		}
		midx = midx / 3; midy = midy / 3; midz = midz / 3;
		if (distance >= getDistance(midx, midy, midz)){
			distance = getDistance(midx, midy, midz);
			PointedFaceIdx = f_it.handle().idx();
		}
	}
	//cout << PointedFaceIdx;
}

float GLFramework::getDistance(GLfloat verx, GLfloat very, GLfloat verz){
	return pow((objX - verx)*(objX - verx) + (objY - very)*(objY - very) + (objZ - verz)*(objZ - verz),
		0.5);
}

float GLFramework::getDistance(float x1, float y1, float z1, float x2, float y2, float z2){
	return pow((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2),
		0.5);
}

void GLFramework::SaveCurrentVertex(){
	
	//std::cout << printf("save current vertex function called");
	//selectPoints.push_back(result.handle().idx());
}

void GLFramework::SaveCurrentFace(){
	selectedFaces.push_back(PointedFaceIdx);
}

void GLFramework::selectVertex(){
	
	for (std::vector<int>::iterator iter = selectedFaces.begin();
		iter != selectedFaces.end();
		iter++){
		OMT::FIter f_it;
		FindFaceInMTriMesh(*iter,f_it);
		for (OMT::FVIter fv_it = mTriMesh->fv_iter(f_it);
			fv_it;
			fv_it++){
			if (!isRecorded(fv_it.handle().idx(), selectPoints))
				selectPoints.push_back(fv_it.handle().idx());
		}
	}
}

void GLFramework::selectBorder(){
	/*for (std::vector<int>::iterator iter = selectPoints.begin();
		iter != selectPoints.end();
		iter++){
		for (OMT::VIter v_it = mTriMesh->vertices_begin();
			v_it != mTriMesh->vertices_end();
			v_it++){
			if (v_it.handle().idx() == *iter){	//use stored index 
				// to find the selectPoint on the mesh
				//then get it's one ring tracing
				bool isBorder = false;
				for (OMT::VVIter vv_it = mTriMesh->vv_iter(v_it);
					vv_it;
					++vv_it){
					bool isIn = false;
					for (std::vector<int>::iterator inner_iter = selectPoints.begin();
						inner_iter != selectPoints.end();
						inner_iter++){
						if (vv_it.handle().idx() == *inner_iter)
							isIn = true;
					}
					isBorder = isBorder || !isIn;
				}
				for (int index = 0; index < 47; index++)
					if (*iter == bottom[index])
						isBorder = true;
				if (isBorder)
					border.push_back(*iter);
			}
		}
	}
	std::cout << endl << "selectPoints index:";
	for (std::vector<int>::iterator iter = selectPoints.begin();
		iter != selectPoints.end();
		iter++){
		cout << ", " << *iter;
	}
	std::cout << endl << "borderPoints index:";
	for (std::vector<int>::iterator iter = border.begin();
		iter != border.end();
		iter++){
		cout << ", " << *iter;
	}
	std::cout << endl << "sorted Points index:";
	for (std::vector<int>::iterator iter = sortedBorder.begin();
		iter != sortedBorder.end();
		iter++){
		cout << ", " << *iter;
	}*/
}
void GLFramework::drawBorder(){
	float decrease = 0.0f;
	/*
	for (std::vector<int>::iterator iter = border.begin();
		iter != border.end();
		iter++){
		decrease += 1.0f;
		for (OMT::VIter v_it = mTriMesh->vertices_begin();
			v_it != mTriMesh->vertices_end();
			v_it++){
			if (v_it.handle().idx() == *iter){
				glPushMatrix();
				auto edge = mTriMesh->point(v_it.handle());
				glTranslatef(edge.data()[0], edge.data()[1], edge.data()[2]);
				GLUquadricObj *sphere;
				sphere = gluNewQuadric();
				glColor3f(0.0f, 1.0f/decrease, 0.0f);
				gluSphere(sphere, 0.4, 20, 20);
				gluDeleteQuadric(sphere);
				glPopMatrix();
			}
		}
	}*/

	for (int index = 0; index < sortedBorder.size(); index++){
		decrease += 1.0;
		OMT::VIter bd;
		FindPointInMTriMesh(sortedBorder[index],bd);
		glPushMatrix();
		auto edge = mTriMesh->point(bd.handle());
		glTranslatef(edge.data()[0], edge.data()[1], edge.data()[2]);
		GLUquadricObj *sphere;
		sphere = gluNewQuadric();
		glColor3f(0.0f, 1.0f / decrease, 0.0f);
		gluSphere(sphere, 0.4, 20, 20);
		gluDeleteQuadric(sphere);
		glPopMatrix();
	}
	
	
}

void GLFramework::sortBorder(){
	//===========================
	/*find a boder in selectPoints and set it
	*as the start of the border */
	bool firstBorderVertexIsFind = false;
	for (std::vector<int>::iterator iter = selectPoints.begin();
		iter != selectPoints.end();
		iter++){
		OMT::VIter v_it;
		FindPointInMTriMesh(*iter, v_it);
		for (OMT::VVIter vv_it = mTriMesh->vv_iter(v_it);
			vv_it;
			vv_it++){
			if (!isRecorded(vv_it.handle().idx(), selectPoints)){
				firstBorderVertexIsFind = true;
				sortedBorder.push_back(*iter);
				break;
			}
		}
		if (firstBorderVertexIsFind)
			break;
	}
	//============================
	/*use the first border vertex and halfedge to find
	*the rest vertex of the border as a sequence*/
	/*find the seconde point*/
	int start = sortedBorder[0];
	int now;
	int previous;
	now = start;
	//int current = start;
	OMT::VIter current;
	FindPointInMTriMesh(start, current);
	for (OMT::VOHEIter v_oeit = mTriMesh->voh_iter(current);
		v_oeit;
		v_oeit++){
		bool flag1, flag2;
		flag1 = isRecorded(mTriMesh->face_handle(v_oeit.handle()).idx(), selectedFaces)
			|| mTriMesh->is_boundary(v_oeit.handle());
		cout << "flag1: " << flag1;
		flag2 = isRecorded(mTriMesh->opposite_face_handle(v_oeit.handle()).idx(), selectedFaces)
			|| mTriMesh->is_boundary(v_oeit.handle());
		cout << "flag2: " << flag2 << endl;
		if (XOR(flag1, flag2)){
			cout << "XOR function working";
			previous = now;
			//TO-DO
			now = mTriMesh->to_vertex_handle(v_oeit.handle()).idx();
			sortedBorder.push_back(now);
			break;
		}
	}
	cout << sortedBorder.size();
	/*find the rest border points*/
	while (now != start){
		FindPointInMTriMesh(now, current);
		for (OMT::VOHEIter v_oeit = mTriMesh->voh_iter(current);
			v_oeit;
			v_oeit++){
			bool flag1, flag2;
			flag1 = isRecorded(mTriMesh->face_handle(v_oeit.handle()).idx(), selectedFaces)
				|| mTriMesh->is_boundary(v_oeit.handle());
			cout << "flag1: " << flag1;
			flag2 = isRecorded(mTriMesh->opposite_face_handle(v_oeit.handle()).idx(), selectedFaces)
				|| mTriMesh->is_boundary(v_oeit.handle());
			cout << "flag2: " << flag2 << endl;
			if (XOR(flag1, flag2) &&
				previous != mTriMesh->to_vertex_handle(v_oeit.handle()).idx()){
				cout << "XOR function working";
				previous = now;
				//TO-DO
				now = mTriMesh->to_vertex_handle(v_oeit.handle()).idx();
				if (now != start)
					sortedBorder.push_back(now);
				break;
			}
		}
	}

	for (int index = 0; index < sortedBorder.size(); index++)
	{
		cout << " " << sortedBorder[index] << " ";
	}
}

bool GLFramework::XOR(bool flag1, bool flag2){
	if (flag1 == flag2)
		return false;
	return true;
}

void GLFramework::FindPointInMTriMesh(int index,OMT::VIter & v_it){
	for (v_it = mTriMesh->vertices_begin();
		v_it != mTriMesh->vertices_end();
		v_it++){
		if (v_it.handle().idx() == index)
			break;
	}
}



bool GLFramework::IsInTheBorder(int index){
	for (std::vector<int>::iterator iter = border.begin();
		iter != border.end();
		iter++){
		if (*iter == index)
			return true;
	}
	return false;
}

void GLFramework::drawSelectedVertex(){
	selectVertex();
	for (std::vector<int>::iterator iter = selectPoints.begin();
		iter != selectPoints.end();
		iter++){
		OMT::VIter v_it;
		FindPointInMTriMesh(*iter, v_it);

		glPushMatrix();
		auto edge = mTriMesh->point(v_it.handle());
		glTranslatef(edge.data()[0], edge.data()[1], edge.data()[2]);
		GLUquadricObj *sphere;
		sphere = gluNewQuadric();
		glColor3f(0.0f, 1.0f, 0.0f);
		gluSphere(sphere, 0.2, 20, 20);
		gluDeleteQuadric(sphere);
		glPopMatrix();
	}
}

void GLFramework::drawCurrentFace(){
	OMT::FIter current;
	FindFaceInMTriMesh(PointedFaceIdx,current);
	glBegin(GL_TRIANGLES);
	for (OMT::FVIter fv_it = mTriMesh->fv_iter(current);
		fv_it;
		fv_it++){
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3dv(mTriMesh->point(fv_it.handle()).data());
	}
	glEnd();
}

void GLFramework::FindFaceInMTriMesh(int index, OMT::FIter & f_it){
	for (f_it = mTriMesh->faces_begin();
		f_it != mTriMesh->faces_end();
		f_it++){
		if (f_it.handle().idx() == index)
			break;
	}
}

void GLFramework::drawStoredFace(){
	OMT::FIter f_it;
	for (std::vector<int>::iterator iter = selectedFaces.begin();
		iter != selectedFaces.end();
		iter++){
		FindFaceInMTriMesh(*iter, f_it);
		glBegin(GL_TRIANGLES);
		for (OMT::FVIter fv_it = mTriMesh->fv_iter(f_it);
			fv_it;
			fv_it++){
			glColor3f(0.0f,1.0f,0.0f);
			glVertex3dv(mTriMesh->point(fv_it.handle()).data());
		}
		glEnd();
	}
}

bool GLFramework::isRecorded(int index, std::vector<int> & memory){
	for (std::vector<int>::iterator iter = memory.begin();
		iter != memory.end();
		iter++){
		if (index == *iter)
			return true;
	}
	return false;
}

void GLFramework::calculate2Dcoordinates(){
	float borderLength = calculateBorderLengthIn3DMesh();
	myPoint start;
	start.idxInMesh = sortedBorder[0];
	start._2dx = 0.0f;
	start._2dy = 0.0f;
	_2DPoints.push_back(start);
	float currentLength = 0.0f;
	for (int index = 1; index < sortedBorder.size(); index++){
		OMT::VIter previous;
		OMT::VIter now;
		FindPointInMTriMesh(sortedBorder[index-1],previous);
		FindPointInMTriMesh(sortedBorder[index], now);
		auto point1 = mTriMesh->point(previous.handle());
		auto point2 = mTriMesh->point(now.handle());
		currentLength += getDistance(point1.data()[0], point1.data()[1], point1.data()[2],
			point2.data()[0], point2.data()[1],point2.data()[2]);
		float OnToTemp = currentLength / borderLength * 4;
		myPoint currentPoint;
		currentPoint.idxInMesh = sortedBorder[index];
		//decide 2D coordinate
		if (OnToTemp <= 1)
		{
			currentPoint._2dx = OnToTemp;
			currentPoint._2dy = 0.0f;
		}
		else if (OnToTemp > 1 && OnToTemp <= 2)
		{
			currentPoint._2dx = 1.0f;
			currentPoint._2dy = OnToTemp - 1.0f;
		}
		else if (OnToTemp > 2 && OnToTemp <= 3)
		{
			currentPoint._2dx = 1.0f- (OnToTemp-2.0f);
			currentPoint._2dy = 1.0f;
		}
		else if (OnToTemp > 3)
		{
			currentPoint._2dx = 0.0f;
			currentPoint._2dy = 1.0f - (OnToTemp - 3.0f);
		}
		_2DPoints.push_back(currentPoint);
	}
	for (int index = 0; index < _2DPoints.size(); index++){
		cout << "x " << _2DPoints[index]._2dx << " y " << _2DPoints[index]._2dy << endl;
	}
}

float GLFramework::calculateBorderLengthIn3DMesh(){
	float length = 0;
	for (int index = 0; index < sortedBorder.size() - 1; index++){
		OMT::VIter current;
		OMT::VIter next;
		FindPointInMTriMesh(sortedBorder[index], current);
		FindPointInMTriMesh(sortedBorder[index+1], next);
		auto point1 = mTriMesh->point(current.handle());
		auto point2 = mTriMesh->point(next.handle());
		length += getDistance(point1.data()[0], point1.data()[1], point1.data()[2],
			point2.data()[0],point2.data()[1], point2.data()[2]);
	}
	OMT::VIter start, end;
	FindPointInMTriMesh(sortedBorder[0], start);
	FindPointInMTriMesh(sortedBorder[sortedBorder.size() - 1], end);
	auto startpoint = mTriMesh->point(start.handle());
	auto endpoint = mTriMesh->point(end.handle());
	length += getDistance(startpoint.data()[0], startpoint.data()[1], startpoint.data()[2],
		endpoint.data()[0], endpoint.data()[1], endpoint.data()[2]);
	return length;
}