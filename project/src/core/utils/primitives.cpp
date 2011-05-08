#include <GL/glew.h>
#include "core/utils/primitives.h"
#include "core/maths/trigo.h"

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

	//    v6----- v5
	//   /|      /|
	//  v1------v0|
	//  | |     | |
	//  | |v7---|-|v4
	//  |/      |/
	//  v2------v3

	// vertex coords array
	const GLfloat vertices[] = {
		 0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5,-0.5, 0.5,  0.5,-0.5, 0.5, // v0-v1-v2-v3
		 0.5, 0.5, 0.5,  0.5,-0.5, 0.5,  0.5,-0.5,-0.5,  0.5, 0.5,-0.5, // v0-v3-v4-v5
		 0.5, 0.5, 0.5,  0.5, 0.5,-0.5, -0.5, 0.5,-0.5, -0.5, 0.5, 0.5, // v0-v5-v6-v1
		-0.5, 0.5, 0.5, -0.5, 0.5,-0.5, -0.5,-0.5,-0.5, -0.5,-0.5, 0.5, // v1-v6-v7-v2
		-0.5,-0.5,-0.5,  0.5,-0.5,-0.5,  0.5,-0.5, 0.5, -0.5,-0.5, 0.5, // v7-v4-v3-v2
		 0.5,-0.5,-0.5, -0.5,-0.5,-0.5, -0.5, 0.5,-0.5,  0.5, 0.5,-0.5  // v4-v7-v6-v5
	};

	// normal array
	const GLfloat normals[] = {
		 0, 0, 1,  0, 0, 1,  0, 0, 1,  0, 0, 1, // v0-v1-v2-v3
		 1, 0, 0,  1, 0, 0,  1, 0, 0,  1, 0, 0, // v0-v3-v4-v5
		 0, 1, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0, // v0-v5-v6-v1
		-1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, // v1-v6-v7-v2
		 0,-1, 0,  0,-1, 0,  0,-1, 0,  0,-1, 0, // v7-v4-v3-v2
		 0, 0,-1,  0, 0,-1,  0, 0,-1,  0, 0,-1  // v4-v7-v6-v5
	};

	// normal array
	const GLfloat texcoords[] = {
		 1, 1,  0, 1,  0, 0,  1, 0, // v0-v1-v2-v3
		 1, 1,  0, 1,  0, 0,  1, 0, // v0-v3-v4-v5
		 1, 1,  0, 1,  0, 0,  1, 0, // v0-v5-v6-v1
		 1, 1,  0, 1,  0, 0,  1, 0, // v1-v6-v7-v2
		 1, 1,  0, 1,  0, 0,  1, 0, // v7-v4-v3-v2
		 1, 1,  0, 1,  0, 0,  1, 0  // v4-v7-v6-v5
	};

	// Set up Vertices VBO
	glGenBuffers(1,&ret->m_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, ret->m_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set up Normals VBO
	glGenBuffers(1,&ret->m_normals);
	glBindBuffer(GL_ARRAY_BUFFER, ret->m_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

	glGenBuffers(1,&ret->m_texcoords);
	glBindBuffer(GL_ARRAY_BUFFER, ret->m_texcoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);

	ret->m_nbFaces = 2*3*6;

	GLshort* indices = new GLshort[ret->m_nbFaces]();
	GLshort* ptrIndices = indices;
	for(int i=0 ; i<6 ; i++) {
		for(int j=0 ; j<3 ; j++) {
			*ptrIndices = 4*i+j;
			ptrIndices++;
		}
		for(int j=0 ; j<4 ; j++) {
			if(j == 1) continue;
			*ptrIndices = 4*i+j;
			ptrIndices++;
		}
	}

	glGenBuffers(1,&ret->m_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret->m_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort)*ret->m_nbFaces, indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indices;

	ret->m_state = STATE_LOADED;

	#ifdef _DEBUG
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			const char* msg = (char*)gluErrorString(error);
			logError( QString(msg) );
		}
	#endif

	return ret;
}

PrimitiveMesh* PrimitiveMesh::buildSphere(int rings, int segments)
{
	PrimitiveMesh* ret = new PrimitiveMesh(QString("Sphere_")+QString().setNum(rings)+QString("_")+QString().setNum(segments));
/*
	ret->m_NormalsArePerVertex = true;
	ret->m_nbVertex = (rings + 1) * (segments+1);

	ret->m_vertices = new float[3*ret->m_nbVertex]();
	ret->m_normals = new float[3*ret->m_nbVertex]();
	ret->m_texcoords = new float[2*ret->m_nbVertex]();

	// Vertex
	for(int r=0 ; r<rings+1 ; r++) {
		for(int s=0 ; s<segments+1 ; s++) {
			int index = (s + r * (segments+1));
			float phi = 360 * (float(s)/segments);
			float theta = 180 * (float(r)/rings);
			float x = fastSin(theta) * fastCos(phi);
			float y = fastCos(theta);
			float z = fastSin(theta) * fastSin(phi);
			// Vertex
			ret->m_vertices[3*index + 0] = 0.5 *  x;
			ret->m_vertices[3*index + 1] = 0.5 *  y;
			ret->m_vertices[3*index + 2] = 0.5 *  z;

			// Normal
			ret->m_normals[3*index + 0] = x;
			ret->m_normals[3*index + 1] = y;
			ret->m_normals[3*index + 2] = z;

			// Texcoord
			ret->m_texcoords[2*index + 0] = float(s)/segments;
			ret->m_texcoords[2*index + 1] = float(r)/rings;
		}
	}

	ret->m_nbFaces = rings * segments;

	ret->m_faces = new Face[ret->m_nbFaces]();

	for(int r=0 ; r<rings ; r++) {
		for(int s=0 ; s<segments ; s++) {
		int i = s + r * segments;
		ret->m_faces[i].nbIndices = 4;
		ret->m_faces[i].indices = new int[4]();

		ret->m_faces[i].indices[0] = s   +  r    * (segments+1);
		ret->m_faces[i].indices[1] = s+1 +  r    * (segments+1);
		ret->m_faces[i].indices[2] = s+1 + (r+1) * (segments+1);
		ret->m_faces[i].indices[3] = s   + (r+1) * (segments+1);
		}
	}

	ret->m_state = STATE_LOADED;
*/
	return ret;
}

PrimitiveMesh* PrimitiveMesh::buildPlane()
{
	PrimitiveMesh* ret = new PrimitiveMesh("Plane");
/*
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
*/
	return ret;
}
