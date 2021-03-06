#include "openmeshframework.h"
#include <algorithm>
#include <QDebug>
#include <QtMath>

namespace OMT 
{
    MyTriMesh::MyTriMesh()
    {
        request_vertex_status();
        request_edge_status();
        request_face_status();
    }
    
    MyTriMesh::~MyTriMesh()
    {
        release_vertex_status();
        release_edge_status();
        release_face_status();
    }
}

bool LoadMesh(OMT::MyTriMesh* mesh, std::string filename)
{
    bool isRead = false;
    OpenMesh::IO::Options opt;
    if ( OpenMesh::IO::read_mesh(*mesh, filename, opt) )
    {
        //read mesh from filename OK!
        isRead = true;
    }
    if(isRead)
    {
        // If the file did not provide vertex normals and mesh has vertex normal ,then calculate them
        if (!opt.check( OpenMesh::IO::Options::VertexNormal ) && mesh->has_vertex_normals())
        {
            mesh->update_normals();
        }
    }
	
    return isRead;
}
using namespace LinearSystemLib;
void SolveAXEqualsB()
{
	GeneralSparseMatrix GA;
	GA.Create(3, 3);
	GA.SetElement(0, 0, 4.0);
	GA.SetElement(0, 1, 3.0);
	GA.SetElement(0, 2, 3.0);
	GA.SetElement(1, 0, 4.0);
	GA.SetElement(1, 1, 2.0);
	GA.SetElement(1, 2, 5.0);
	GA.SetElement(2, 0, 8.0);
	GA.SetElement(2, 1, 8.0);
	GA.SetElement(2, 2, 10.0);
	
	const int dim = 1;
	double** B = new double*[dim];
	B[0] = new double[3]; 
	B[0][0] = 350.0;
	B[0][1] = 360.0;
	B[0][2] = 840.0;

	SparseLinearSystem sls(new StableSparseMatrix(GA), B, dim);
	double** x = 0;
	bool result = LeastSquareSparseLSSolver::GetInstance()->Solve(&sls, x);

	std::cout << "GA:" << std::endl;
	for (int i = 0; i < GA.GetNumRows(); i++)
	{
		for (int j = 0; j < GA.GetNumCols(); j++)
		{
			std::cout << GA.GetElement(i, j) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "B:" << std::endl;
	std::cout << B[0][0] << std::endl;
	std::cout << B[0][1] << std::endl;
	std::cout << B[0][2] << std::endl << std::endl;

	std::cout << "x:" << std::endl;
	std::cout << x[0][0] << std::endl;
	std::cout << x[0][1] << std::endl;
	std::cout << x[0][2] << std::endl;


}