#include "core/resources/assimpmesh.h"
#include <QtOpenGL>

#include <assimp.h>
#include <aiMesh.h>

AssimpMesh::AssimpMesh(const QString& name, const QString& path, IResourceFactory* factory, const aiMesh* mesh) :
	MeshData(name,path,factory),
	m_mesh(mesh),
	m_vertices(),
	m_normals(),
	m_colors(),
	m_texcoords(),
	m_tangents(),
	m_bitangents(),
	m_indices(QGLBuffer::IndexBuffer),
	m_nbFaces(0)
{
	buildVBO();
}

bool AssimpMesh::unload()
{
	return false;
}

void AssimpMesh::buildVBO()
{
	m_nbFaces = 0;

	bool error = false;
	for (unsigned t = 0; t < m_mesh->mNumFaces; ++t) {
		const struct aiFace* face = &m_mesh->mFaces[t];
		switch(face->mNumIndices) {
			case 3:
				m_nbFaces += 3;
				break;
			case 4:
				m_nbFaces += 6;
				break;
			default:
				error = true;
				break;
		}
	}
	if(error) {
		logError("Assimp Mesh" << name() << "contains faces that are neither triangles or quad, thay will not display");
	}

	if(m_mesh->mVertices != NULL) {
		m_vertices.create();
		m_vertices.bind();
		m_vertices.setUsagePattern(QGLBuffer::StaticDraw);
		GLfloat* array = new GLfloat[3 * m_mesh->mNumVertices]();
		for(unsigned int i=0 ; i < m_mesh->mNumVertices ; i++) {
			memcpy(&(array[3*i]),&m_mesh->mVertices[i].x,3 * sizeof(GLfloat));
		}
		m_vertices.allocate(array,3 * sizeof(GLfloat) * m_mesh->mNumVertices);
		delete[] array;
	}

	if(m_mesh->mNormals != NULL) {
		m_normals.create();
		m_normals.bind();
		m_normals.setUsagePattern(QGLBuffer::StaticDraw);
		GLfloat* array = new GLfloat[3 * m_mesh->mNumVertices]();
		for(unsigned int i=0 ; i < m_mesh->mNumVertices ; i++) {
			memcpy(&(array[3*i]),&m_mesh->mNormals[i].x,3 * sizeof(GLfloat));
		}
		m_normals.allocate(array,3 * sizeof(GLfloat) * m_mesh->mNumVertices);
		delete[] array;
	}

	if(m_mesh->mColors[0] != NULL) {
		m_colors.create();
		m_colors.bind();
		m_colors.setUsagePattern(QGLBuffer::StaticDraw);
		GLfloat* array = new GLfloat[4 * m_mesh->mNumVertices]();
		for(unsigned int i=0 ; i < m_mesh->mNumVertices ; i++) {
			array[0 + 4*i] = m_mesh->mColors[0][i].r;
			array[0 + 4*i] = m_mesh->mColors[0][i].g;
			array[0 + 4*i] = m_mesh->mColors[0][i].b;
			array[0 + 4*i] = m_mesh->mColors[0][i].a;
		}
		m_colors.allocate(array,4 * sizeof(GLfloat) * m_mesh->mNumVertices);
		delete[] array;
	}

	if(m_mesh->mTextureCoords[0] != NULL) {
		m_texcoords.create();
		m_texcoords.bind();
		m_texcoords.setUsagePattern(QGLBuffer::StaticDraw);
		GLfloat* array = new GLfloat[2 * m_mesh->mNumVertices]();
		for(unsigned int i=0 ; i < m_mesh->mNumVertices ; i++) {
			memcpy(&(array[2*i]),&m_mesh->mTextureCoords[0][i].x,2 * sizeof(GLfloat));
		}
		m_texcoords.allocate(array,2 * sizeof(GLfloat) * m_mesh->mNumVertices);
		delete[] array;
	}

	if(m_mesh->mTangents != NULL) {
		m_tangents.create();
		m_tangents.bind();
		m_tangents.setUsagePattern(QGLBuffer::StaticDraw);
		GLfloat* array = new GLfloat[3 * m_mesh->mNumVertices]();
		for(unsigned int i=0 ; i < m_mesh->mNumVertices ; i++) {
			memcpy(&(array[3*i]),&m_mesh->mTangents[i].x,3 * sizeof(GLfloat));
		}
		m_tangents.allocate(array,3 * sizeof(GLfloat) * m_mesh->mNumVertices);
		delete[] array;
	}

	if(m_mesh->mBitangents != NULL) {
		m_bitangents.create();
		m_bitangents.bind();
		m_bitangents.setUsagePattern(QGLBuffer::StaticDraw);
		GLfloat* array = new GLfloat[3 * m_mesh->mNumVertices]();
		for(unsigned int i=0 ; i < m_mesh->mNumVertices ; i++) {
			memcpy(&(array[3*i]),&m_mesh->mBitangents[i].x,3 * sizeof(GLfloat));
		}
		m_bitangents.allocate(array,3 * sizeof(GLfloat) * m_mesh->mNumVertices);
		delete[] array;
	}

	GLshort* indices = new GLshort[3 * m_nbFaces]();
	int index = 0;
	for (unsigned t = 0; t < m_mesh->mNumFaces; ++t) {
		const struct aiFace* face = &m_mesh->mFaces[t];
		switch(face->mNumIndices) {
			case 3:
				indices[index  ] = face->mIndices[0];
				indices[index+1] = face->mIndices[1];
				indices[index+2] = face->mIndices[2];
				index += 3;
				break;
			case 4:
				indices[index  ] = face->mIndices[0];
				indices[index+1] = face->mIndices[1];
				indices[index+2] = face->mIndices[2];
				index += 3;
				indices[index  ] = face->mIndices[0];
				indices[index+1] = face->mIndices[2];
				indices[index+2] = face->mIndices[3];
				index += 3;
				break;
			default:
				break;
		}
	}
	m_indices.create();
	m_indices.bind();
	m_indices.setUsagePattern(QGLBuffer::StaticDraw);
	m_indices.allocate(indices,3 * sizeof(GLshort) * m_nbFaces);
	delete[] indices;

	// Unbind buffers (binding to a non created buffer does the trick)
	//QGLBuffer().bind();
	//QGLBuffer(QGLBuffer::IndexBuffer).bind();
}

void AssimpMesh::draw(QGLShaderProgram* program)
{
	if(!m_vertices.isCreated() || !m_indices.isCreated())
		return;

	if(m_texcoords.isCreated())
	{
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		m_texcoords.bind();
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	if(m_colors.isCreated())
	{
		glEnable(GL_COLOR_MATERIAL);
		glEnableClientState(GL_COLOR_ARRAY);
		m_colors.bind();
		glColorPointer(4, GL_FLOAT, 0, NULL);
	}
	else
	{
		glDisable(GL_COLOR_MATERIAL);
	}

	if(m_normals.isCreated())
	{
		glEnable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		glEnableClientState(GL_NORMAL_ARRAY);
		m_normals.bind();
		glNormalPointer(GL_FLOAT, 0, NULL);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}

	if(program != NULL)
	{
		int location = program->attributeLocation("tangent");
		if(location != -1)
		{
			m_tangents.bind();
			program->enableAttributeArray(location);
			program->setAttributeBuffer(location,GL_FLOAT,0,3);
		}
		location = program->attributeLocation("bitangent");
		if(location != -1)
		{
			m_bitangents.bind();
			program->enableAttributeArray(location);
			program->setAttributeBuffer(location,GL_FLOAT,0,3);
		}
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	m_vertices.bind();
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glEnableClientState(GL_INDEX_ARRAY);
	m_indices.bind();
	glIndexPointer(GL_SHORT, 0, NULL);

	glDrawElements(GL_TRIANGLES, 3*m_nbFaces, GL_UNSIGNED_SHORT, NULL);

	m_indices.release();
	m_vertices.release();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);

	debugGL("while rendering" << name());
}
