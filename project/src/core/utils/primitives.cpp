#include "core/utils/primitives.h"

PrimitiveMesh::PrimitiveMesh(QString name) :
    RawMesh(name,"",NULL)
{
}

void PrimitiveMesh::load()
{
}

bool PrimitiveMesh::unload()
{
    return false;
}

PrimitiveMesh* PrimitiveMesh::buildCube()
{
    PrimitiveMesh* ret = new PrimitiveMesh("Cube");

    ret->m_nbVertex = 8;

    ret->m_state = STATE_LOADED;

    return ret;
}

PrimitiveMesh* PrimitiveMesh::buildSphere(int rings, int segments)
{
    PrimitiveMesh* ret = new PrimitiveMesh(QString("Sphere")+QString().setNum(rings)+QString("-")+QString().setNum(segments));

    ret->m_nbVertex = 2 + (rings - 2) * segments;

    ret->m_state = STATE_LOADED;

    return ret;
}

PrimitiveMesh* PrimitiveMesh::buildPlane()
{
    PrimitiveMesh* ret = new PrimitiveMesh("Plane");

    ret->m_nbVertex = 4;
    ret->m_vertices = new float[4*3]();
    ret->m_normals = new float[4*3]();
    ret->m_texcoords = new float[4*2]();

    const float vertices[] = {
        -0.5, 0, -0.5,
        -0.5, 0,  0.5,
         0.5, 0,  0.5,
         0.5, 0, -0.5
    };

    const float normals[] = {
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0
    };

    const float texcoords[] = {
        0, 0,
        0, 1,
        1, 1,
        1, 0
    };

    memcpy(ret->m_vertices,vertices,sizeof(vertices));
    memcpy(ret->m_normals,normals,sizeof(normals));
    memcpy(ret->m_texcoords,texcoords,sizeof(texcoords));

    ret->m_nbFaces = 1;

    ret->m_faces = new Face[1]();

    ret->m_faces[0].nbIndices = 4;
    ret->m_faces[0].indices = new int[4]();

    const int indices[] =  { 0, 1, 2, 3 };

    memcpy(ret->m_faces[0].indices,indices,sizeof(indices));

    ret->m_state = STATE_LOADED;

    return ret;
}
