#ifndef OPENMESHFRAMEWORK
#define OPENMESHFRAMEWORK

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Tools/Utils/getopt.h>
#include <QOpenGLFunctions_1_1>
#include <LinearSystemLib.h>
#include <iostream>

namespace OMT // OpenMesh triangle mesh 
{
    /* Define custom traits */
    struct MyTraits : OpenMesh::DefaultTraits
    {
        // Define point and normal as double
        typedef OpenMesh::Vec3d Point;
        typedef OpenMesh::Vec3d Normal;
        
        // Add normal property to vertices and faces
        VertexAttributes(OpenMesh::Attributes::Normal);
        FaceAttributes  (OpenMesh::Attributes::Normal);
    };
    
    /* Define commonly used types */
    /* Basic types */
    typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits>    MyMesh;
    typedef OpenMesh::Vec3d                             Vec3d;
    typedef MyMesh::Scalar								Scalar;
    typedef MyMesh::Point								Point;
    typedef MyMesh::Normal								Normal;
    /* Handles */
    typedef MyMesh::VertexHandle						VHandle;
    typedef MyMesh::HalfedgeHandle						HEHandle;
    typedef MyMesh::EdgeHandle							EHandle;
    typedef MyMesh::FaceHandle							FHandle;
    /* Iterators */
    typedef MyMesh::VertexIter							VIter;
    typedef MyMesh::VertexVertexIter					VVIter;
    typedef MyMesh::VertexEdgeIter						VEIter;
    typedef MyMesh::VertexFaceIter						VFIter;
    typedef MyMesh::EdgeIter							EIter;
    typedef MyMesh::FaceIter							FIter;
    typedef MyMesh::HalfedgeIter                        HEIter;
    typedef MyMesh::FaceVertexIter						FVIter;
    typedef MyMesh::FaceEdgeIter						FEIter;
    typedef MyMesh::FaceFaceIter						FFIter;
    typedef MyMesh::VertexOHalfedgeIter					VOHEIter;
    typedef MyMesh::ConstVertexVertexIter				CVVIter;
    
    class MyTriMesh : public MyMesh
    {
    public:
        MyTriMesh();      // Constructor
        ~MyTriMesh();     // De-constructor
    };
}

bool LoadMesh(OMT::MyTriMesh* mesh, std::string filename);
void SolveAXEqualsB();

#endif // OPENMESHFRAMEWORK
