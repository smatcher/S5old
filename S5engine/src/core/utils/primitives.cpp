#include <QtOpenGL>
#include "core/utils/primitives.h"
#include "core/maths/trigo.h"
#include "core/maths/aabb.h"
#include "core/maths/boundingsphere.h"

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

	// tangent array
	const GLfloat tangents[] = {
		 1, 0, 0,  1, 0, 0,  1, 0, 0,  1, 0, 0, // v0-v1-v2-v3
		 0, 0,-1,  0, 0,-1,  0, 0,-1,  0, 0,-1, // v0-v3-v4-v5
		 1, 0, 0,  1, 0, 0,  1, 0, 0,  1, 0, 0, // v0-v5-v6-v1
		 0, 0, 1,  0, 0, 1,  0, 0, 1,  0, 0, 1, // v1-v6-v7-v2
		 0, 0, 1,  0, 0, 1,  0, 0, 1,  0, 0, 1, // v7-v4-v3-v2
		-1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0  // v4-v7-v6-v5
	};

	// bitangent array
	const GLfloat bitangents[] = {
		 0, 1, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0, // v0-v1-v2-v3
		 0, 1, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0, // v0-v3-v4-v5
		 0, 0,-1,  0, 0,-1,  0, 0,-1,  0, 0,-1, // v0-v5-v6-v1
		 0, 1, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0, // v1-v6-v7-v2
		 0, 0, 1,  0, 0, 1,  0, 0, 1,  0, 0, 1, // v7-v4-v3-v2
		 0, 1, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0  // v4-v7-v6-v5
	};

	// texcoord array
	const GLfloat texcoords[] = {
		 1, 1,  0, 1,  0, 0,  1, 0, // v0-v1-v2-v3
		 0, 1,  0, 0,  1, 0,  1, 1, // v0-v3-v4-v5
		 1, 0,  1, 1,  0, 1,  0, 0, // v0-v5-v6-v1
		 1, 1,  0, 1,  0, 0,  1, 0, // v1-v6-v7-v2
		 0, 0,  1, 0,  1, 1,  0, 1, // v7-v4-v3-v2
		 0, 0,  1, 0,  1, 1,  0, 1  // v4-v7-v6-v5
	};

	// Set up Vertices VBO
	ret->m_vertices.create();
	ret->m_vertices.bind();
	ret->m_vertices.setUsagePattern(QGLBuffer::StaticDraw);
	ret->m_vertices.allocate(vertices, sizeof(vertices));

	// Set up Normals VBO
	ret->m_normals.create();
	ret->m_normals.bind();
	ret->m_normals.setUsagePattern(QGLBuffer::StaticDraw);
	ret->m_normals.allocate(normals, sizeof(normals));

	// Set up Tangents VBO
	ret->m_tangents.create();
	ret->m_tangents.bind();
	ret->m_tangents.setUsagePattern(QGLBuffer::StaticDraw);
	ret->m_tangents.allocate(tangents, sizeof(tangents));

	// Set up Bitangents VBO
	ret->m_bitangents.create();
	ret->m_bitangents.bind();
	ret->m_bitangents.setUsagePattern(QGLBuffer::StaticDraw);
	ret->m_bitangents.allocate(bitangents, sizeof(bitangents));

	// Set up TexCoord VBO
	ret->m_texcoords.create();
	ret->m_texcoords.bind();
	ret->m_texcoords.setUsagePattern(QGLBuffer::StaticDraw);
	ret->m_texcoords.allocate(texcoords, sizeof(texcoords));

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

	ret->m_indices.create();
	ret->m_indices.bind();
	ret->m_indices.setUsagePattern(QGLBuffer::StaticDraw);
	ret->m_indices.allocate(indices, sizeof(GLshort)*3*ret->m_nbFaces);

	ret->m_indices.release();
	ret->m_texcoords.release();

	delete[] indices;

	ret->m_nbVertices = 8;
	ret->m_boundingVolume = new AABB(Vector3f(0,0,0),Vector3f(0.5,0.5,0.5));

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
	GLfloat* tangents = new GLfloat[3*nbVertex]();
	GLfloat* bitangents = new GLfloat[3*nbVertex]();
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
			float tx = fastSin(theta) * fastCos(phi + 90);
			float ty = fastCos(theta);
			float tz = fastSin(theta) * fastSin(phi + 90);
			float bx = fastSin(theta + 90) * fastCos(phi);
			float by = fastCos(theta + 90);
			float bz = fastSin(theta + 90) * fastSin(phi);
			// Vertex
			vertices[3*index + 0] = 0.5 *  x;
			vertices[3*index + 1] = 0.5 *  y;
			vertices[3*index + 2] = 0.5 *  z;

			// Normal
			normals[3*index + 0] = x;
			normals[3*index + 1] = y;
			normals[3*index + 2] = z;

			// Tangents
			tangents[3*index + 0] = tx;
			tangents[3*index + 1] = ty;
			tangents[3*index + 2] = tz;

			// Bitangents
			bitangents[3*index + 0] = bx;
			bitangents[3*index + 1] = by;
			bitangents[3*index + 2] = bz;

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
	ret->m_vertices.create();
	ret->m_vertices.bind();
	ret->m_vertices.setUsagePattern(QGLBuffer::StaticDraw);
	ret->m_vertices.allocate(vertices, sizeof(GLfloat) * nbVertex * 3);

	// Set up Normals VBO
	ret->m_normals.create();
	ret->m_normals.bind();
	ret->m_normals.setUsagePattern(QGLBuffer::StaticDraw);
	ret->m_normals.allocate(normals, sizeof(GLfloat) * nbVertex * 3);

	// Set up Tangents VBO
	ret->m_tangents.create();
	ret->m_tangents.bind();
	ret->m_tangents.setUsagePattern(QGLBuffer::StaticDraw);
	ret->m_tangents.allocate(tangents, sizeof(GLfloat) * nbVertex * 3);

	// Set up Bitangents VBO
	ret->m_bitangents.create();
	ret->m_bitangents.bind();
	ret->m_bitangents.setUsagePattern(QGLBuffer::StaticDraw);
	ret->m_bitangents.allocate(bitangents, sizeof(GLfloat) * nbVertex * 3);

	// Set up Texture Coordinates VBO
	ret->m_texcoords.create();
	ret->m_texcoords.bind();
	ret->m_texcoords.setUsagePattern(QGLBuffer::StaticDraw);
	ret->m_texcoords.allocate(texcoords, sizeof(GLfloat) * nbVertex * 2);

	ret->m_indices.create();
	ret->m_indices.bind();
	ret->m_indices.setUsagePattern(QGLBuffer::StaticDraw);
	ret->m_indices.allocate(indices, sizeof(GLshort) * ret->m_nbFaces * 3);


	ret->m_indices.release();
	ret->m_texcoords.release();

	delete[] vertices;
	delete[] normals;
	delete[] texcoords;
	delete[] indices;

	ret->m_nbVertices = nbVertex;
	ret->m_boundingVolume = new BoundingSphere(Vector3f(0,0,0),0.5);

	ret->m_state = STATE_LOADED;

	debugGL("While build a sphere primitive");

	return ret;
}

PrimitiveMesh* PrimitiveMesh::buildPlane()
{
	PrimitiveMesh* ret = new PrimitiveMesh("Plane");

	const GLfloat vertices[] = {
		-0.5,  0.5, 0,
		-0.5, -0.5, 0,
		 0.5, -0.5, 0,
		 0.5,  0.5, 0
	};

	const GLfloat normals[] = {
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1
	};

	const GLfloat tangents[] = {
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0
	};

	const GLfloat bitangents[] = {
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0
	};

	const GLfloat texcoords[] = {
		0, 1,
		0, 0,
		1, 0,
		1, 1
	};

	const GLshort indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	// Set up Vertices VBO
	ret->m_vertices.create();
	ret->m_vertices.bind();
	ret->m_vertices.setUsagePattern(QGLBuffer::StaticDraw);
	ret->m_vertices.allocate(vertices, sizeof(vertices));

	// Set up Normals VBO
	ret->m_normals.create();
	ret->m_normals.bind();
	ret->m_normals.setUsagePattern(QGLBuffer::StaticDraw);
	ret->m_normals.allocate(normals, sizeof(normals));

	// Set up Tangents VBO
	ret->m_tangents.create();
	ret->m_tangents.bind();
	ret->m_tangents.setUsagePattern(QGLBuffer::StaticDraw);
	ret->m_tangents.allocate(tangents, sizeof(tangents));

	// Set up Bitangents VBO
	ret->m_bitangents.create();
	ret->m_bitangents.bind();
	ret->m_bitangents.setUsagePattern(QGLBuffer::StaticDraw);
	ret->m_bitangents.allocate(bitangents, sizeof(bitangents));

	ret->m_texcoords.create();
	ret->m_texcoords.bind();
	ret->m_texcoords.setUsagePattern(QGLBuffer::StaticDraw);
	ret->m_texcoords.allocate(texcoords, sizeof(texcoords));

	ret->m_nbFaces = 2;

	ret->m_indices.create();
	ret->m_indices.bind();
	ret->m_indices.setUsagePattern(QGLBuffer::StaticDraw);
	ret->m_indices.allocate(indices, sizeof(indices));

	ret->m_indices.release();
	ret->m_texcoords.release();

	ret->m_nbVertices = 4;
	ret->m_boundingVolume = new AABB(Vector3f(0,0,0),Vector3f(0.5,0.5,0.01));

	ret->m_state = STATE_LOADED;

	debugGL("While building a plane primitive");

	return ret;
}
