#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "glframework.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <QStatusBar>
#include <QSpinBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    installEventFilter(this);
    setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    QKeyEvent* e = static_cast<QKeyEvent*>(event);
    switch (event->type())
    {
    case QEvent::KeyPress:
        if (e->key() == Qt::Key_L)
        {
            LoadMeshFile("head.obj");
			//ui->glFramework->floor = ui->glFramework->LoadGLTextures("floor.bmp");
        }
		if (e->key() == Qt::Key_S)
		{
			SolveAXEqualsB();
		}
		if (e->key() == Qt::Key_A)
		{
			ui->glFramework->SaveCurrentVertex();
		}
        return true;
    case QEvent::KeyRelease:
        return true;
    default:
        return false;
    }
}

void MainWindow::LoadMeshFile(QString filename)
{
    // Initiate mesh
	if (ui->glFramework->mTriMesh){
		delete ui->glFramework->mTriMesh;
		std::cout << "mTriMesh deleted and rebuilt" << std::endl;
	}
    ui->glFramework->mTriMesh = new OMT::MyTriMesh();

    // Load mesh
    bool ok = ui->glFramework->LoadMesh(ui->glFramework->mTriMesh, filename.toStdString());



    // Check not invalid mesh
    if (ok && ui->glFramework->mTriMesh->n_vertices() > 0)
    {
        // Loading successful, update OpenGL paint
        ui->glFramework->update();
        // Show successful message
        QString str = "Mesh loaded. " 
            + QString("Vertices loaded: %1. ").arg(ui->glFramework->mTriMesh->n_vertices())
            + QString("Faces loaded: %1. ").arg(ui->glFramework->mTriMesh->n_faces());
        ui->statusBar->showMessage(str);
    }
    else
    {
        // Loading failed, delete mesh
        delete ui->glFramework->mTriMesh;
		std::cout << "MeshDeleted" << std::endl;
        // Show error message
        ui->statusBar->showMessage("Loading failed.");
    }
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_checkBoxLocator_clicked(bool checked)
{
    ui->glFramework->mEnabledLocator = checked;
    ui->glFramework->update();
}

void MainWindow::on_checkBoxGrid_clicked(bool checked)
{
    ui->glFramework->mEnabledGrid = checked;
    ui->glFramework->update();
}

void MainWindow::on_checkBoxTrackball_clicked(bool checked)
{
    ui->glFramework->mTrackBallCamera.enabled = checked;
}
void MainWindow::on_SaveVertex_clicked()
{
	ui->glFramework->SaveCurrentFace();
}

void MainWindow::on_drawBorder_button_clicked()
{
	//std::cout << "this button is not activated";
	ui->glFramework->sortBorder();
	ui->glFramework->calculate2Dcoordinates();
}
