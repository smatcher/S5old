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

	ret->m_NormalsArePerVertex = false;
	ret->m_nbVertex = 8;
	ret->m_vertices = new float[8*3]();
	ret->m_normals = new float[6*3]();
	ret->m_texcoords = new float[8*2]();

	const float vertices[] = {
		-0.5,  0.5, -0.5,
		-0.5,  0.5,  0.5,
		 0.5,  0.5,  0.5,
		 0.5,  0.5, -0.5,
		-0.5, -0.5, -0.5,
		-0.5, -0.5,  0.5,
		 0.5, -0.5,  0.5,
		 0.5, -0.5, -0.5
	};

	const float normals[] = {
		 1,  0,  0,
		 0,  1,  0,
		-1,  0,  0,
		 0, -1,  0,
		 0,  0,  1,
		 0,  0, -1
	};

	const float texcoords[] = {
		0, 0,
		0, 1,
		1, 1,
		1, 0,
		0, 0,
		0, 1,
		1, 1,
		1, 0
	};

	memcpy(ret->m_vertices,vertices,sizeof(vertices));
	memcpy(ret->m_normals,normals,sizeof(normals));
	memcpy(ret->m_texcoords,texcoords,sizeof(texcoords));

	ret->m_nbFaces = 6;

	ret->m_faces = new Face[6]();

	const int indices[][4] = {
		{ 2, 3, 7, 6 },
		{ 0, 1, 2, 3 },
		{ 0, 1, 5, 4 },
		{ 7, 6, 5, 4 },
		{ 1, 2, 6, 5 },
		{ 0, 3, 7, 4 },
	};

	for(int i=0 ; i<6 ; i++)
	{
		ret->m_faces[i].nbIndices = 4;
		ret->m_faces[i].indices = new int[4]();
		memcpy(ret->m_faces[i].indices,indices[i],sizeof(indices[0]));
	}

	ret->m_state = STATE_LOADED;

	return ret;
}

PrimitiveMesh* PrimitiveMesh::buildSphere(int rings, int segments)
{
	PrimitiveMesh* ret = new PrimitiveMesh(QString("Sphere")+QString().setNum(rings)+QString("-")+QString().setNum(segments));

	ret->m_NormalsArePerVertex = true;
	ret->m_nbVertex = 2 + (rings - 2) * segments;

	ret->m_state = STATE_LOADED;

	return ret;
}

PrimitiveMesh* PrimitiveMesh::buildPlane()
{
	PrimitiveMesh* ret = new PrimitiveMesh("Plane");

	ret->m_NormalsArePerVertex = false;

	ret->m_nbVertex = 4;
	ret->m_vertices = new float[4*3]();
	ret->m_normals = new float[3]();
	ret->m_texcoords = new float[4*2]();

	const float vertices[] = {
		-0.5, 0, -0.5,
		-0.5, 0,  0.5,
		 0.5, 0,  0.5,
		 0.5, 0, -0.5
	};

	const float normals[] = {
		0, 1, 0,
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
