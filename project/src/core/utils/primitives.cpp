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

	ret->m_nbFaces = 2*6;

	GLshort* indices = new GLshort[3*ret->m_nbFaces]();
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort)*3*ret->m_nbFaces, indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indices;

	ret->m_state = STATE_LOADED;

	debugGL("While building a cube primitive");

	return ret;
}

PrimitiveMesh* PrimitiveMesh::buildSphere(int rings, int segments)
{
	PrimitiveMesh* ret = new PrimitiveMesh(QString("Sphere_")+QString().setNum(rings)+QString("_")+QString().setNum(segments));

	int nbVertex = (rings+1) * (segments+1);

	GLfloat* vertices = new GLfloat[3*nbVertex]();
	GLfloat* normals = new GLfloat[3*nbVertex]();
	GLfloat* texcoords = new GLfloat[2*nbVertex]();

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
			vertices[3*index + 0] = 0.5 *  x;
			vertices[3*index + 1] = 0.5 *  y;
			vertices[3*index + 2] = 0.5 *  z;

			// Normal
			normals[3*index + 0] = x;
			normals[3*index + 1] = y;
			normals[3*index + 2] = z;

			// Texcoord
			texcoords[2*index + 0] = float(s)/segments;
			texcoords[2*index + 1] = float(r)/rings;
		}
	}

	ret->m_nbFaces = 2 * rings * segments;

	GLshort* indices = new GLshort[3* ret->m_nbFaces]();
	GLshort* ptrIndices = indices;

	for(int r=0 ; r<rings ; r++) {
		for(int s=0 ; s<segments ; s++) {

		*ptrIndices = s   +  r    * (segments+1); ptrIndices++;
		*ptrIndices = s+1 +  r    * (segments+1); ptrIndices++;
		*ptrIndices = s+1 + (r+1) * (segments+1); ptrIndices++;

		*ptrIndices = s   +  r    * (segments+1); ptrIndices++;
		*ptrIndices = s+1 + (r+1) * (segments+1); ptrIndices++;
		*ptrIndices = s   + (r+1) * (segments+1); ptrIndices++;

		}
	}

	// Set up Vertices VBO
	glGenBuffers(1,&ret->m_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, ret->m_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * nbVertex * 3, vertices, GL_STATIC_DRAW);

	// Set up Normals VBO
	glGenBuffers(1,&ret->m_normals);
	glBindBuffer(GL_ARRAY_BUFFER, ret->m_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * nbVertex * 3, normals, GL_STATIC_DRAW);

	// Set up Texture Coordinates VBO
	glGenBuffers(1,&ret->m_texcoords);
	glBindBuffer(GL_ARRAY_BUFFER, ret->m_texcoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * nbVertex * 2, texcoords, GL_STATIC_DRAW);


	glGenBuffers(1,&ret->m_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret->m_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort) * ret->m_nbFaces * 3, indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] vertices;
	delete[] normals;
	delete[] texcoords;
	delete[] indices;

	ret->m_state = STATE_LOADED;

	debugGL("While build a sphere primitive");

	return ret;
}

PrimitiveMesh* PrimitiveMesh::buildPlane()
{
	PrimitiveMesh* ret = new PrimitiveMesh("Plane");

	const GLfloat vertices[] = {
		-0.5, 0, -0.5,
		-0.5, 0,  0.5,
		 0.5, 0,  0.5,
		 0.5, 0, -0.5
	};

	const GLfloat normals[] = {
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0
	};

	const GLfloat texcoords[] = {
		0, 0,
		0, 1,
		1, 1,
		1, 0
	};

	const GLshort indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	// Set up Vertices VBO
	glGenBuffers(1,&ret->m_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, ret->m_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set up Normals VBO
	glGenBuffers(1,&ret->m_normals);
	glBindBuffer(GL_ARRAY_BUFFER, ret->m_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

	// Set up Texture Coordinates VBO
	glGenBuffers(1,&ret->m_texcoords);
	glBindBuffer(GL_ARRAY_BUFFER, ret->m_texcoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);

	ret->m_nbFaces = 2;

	glGenBuffers(1,&ret->m_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret->m_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	ret->m_state = STATE_LOADED;

	debugGL("While building a plane primitive");

	return ret;
}
