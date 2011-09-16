#include "core/resources/assimpmesh.h"
#include <QtOpenGL>

#include <assimp.h>
#include <aiMesh.h>

AssimpMesh::AssimpMesh(const QString& name, const QString& path, IResourceFactory* factory) :
	MeshData(name,path,factory),
	m_skeleton(NULL)
{
	m_state = STATE_UNLOADED;
}


bool AssimpMesh::unload()
{
	return false;
}

void AssimpMesh::buildVBOs()
{
	for(int i=0 ; i<m_submeshes.size() ; i++) {
		m_submeshes[i]->buildVBO(name());
	}
}

void AssimpMesh::Submesh::buildVBO(QString name)
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
		logError("Assimp Mesh" << name << "contains faces that are neither triangles or quad, thay will not display");
	}

	if(m_mesh->mVertices != NULL) {
		GLfloat* array = new GLfloat[3 * m_mesh->mNumVertices]();
		for(unsigned int i=0 ; i < m_mesh->mNumVertices ; i++) {
			memcpy(&(array[3*i]),&m_mesh->mVertices[i].x,3 * sizeof(GLfloat));
		}

		m_vertices.create();
		m_vertices.bind();
		m_vertices.setUsagePattern(QGLBuffer::StaticDraw);
		m_vertices.allocate(array,3 * sizeof(GLfloat) * m_mesh->mNumVertices);

		m_skinned_vertices.create();
		m_skinned_vertices.bind();
		m_skinned_vertices.setUsagePattern(QGLBuffer::DynamicDraw);
		m_skinned_vertices.allocate(array,3 * sizeof(GLfloat) * m_mesh->mNumVertices);

		delete[] array;
	}

	if(m_mesh->mNormals != NULL) {
		GLfloat* array = new GLfloat[3 * m_mesh->mNumVertices]();
		for(unsigned int i=0 ; i < m_mesh->mNumVertices ; i++) {
			memcpy(&(array[3*i]),&m_mesh->mNormals[i].x,3 * sizeof(GLfloat));
		}

		m_normals.create();
		m_normals.bind();
		m_normals.setUsagePattern(QGLBuffer::StaticDraw);
		m_normals.allocate(array,3 * sizeof(GLfloat) * m_mesh->mNumVertices);

		m_skinned_normals.create();
		m_skinned_normals.bind();
		m_skinned_normals.setUsagePattern(QGLBuffer::DynamicDraw);
		m_skinned_normals.allocate(array,3 * sizeof(GLfloat) * m_mesh->mNumVertices);

		delete[] array;
	}

	if(m_mesh->mColors[0] != NULL) {
		GLfloat* array = new GLfloat[4 * m_mesh->mNumVertices]();
		for(unsigned int i=0 ; i < m_mesh->mNumVertices ; i++) {
			array[0 + 4*i] = m_mesh->mColors[0][i].r;
			array[0 + 4*i] = m_mesh->mColors[0][i].g;
			array[0 + 4*i] = m_mesh->mColors[0][i].b;
			array[0 + 4*i] = m_mesh->mColors[0][i].a;
		}

		m_colors.create();
		m_colors.bind();
		m_colors.setUsagePattern(QGLBuffer::StaticDraw);
		m_colors.allocate(array,4 * sizeof(GLfloat) * m_mesh->mNumVertices);

		delete[] array;
	}

	if(m_mesh->mTextureCoords[0] != NULL) {
		GLfloat* array = new GLfloat[2 * m_mesh->mNumVertices]();
		for(unsigned int i=0 ; i < m_mesh->mNumVertices ; i++) {
			memcpy(&(array[2*i]),&m_mesh->mTextureCoords[0][i].x,2 * sizeof(GLfloat));
		}

		m_texcoords.create();
		m_texcoords.bind();
		m_texcoords.setUsagePattern(QGLBuffer::StaticDraw);
		m_texcoords.allocate(array,2 * sizeof(GLfloat) * m_mesh->mNumVertices);

		delete[] array;
	}

	if(m_mesh->mTangents != NULL) {
		GLfloat* array = new GLfloat[3 * m_mesh->mNumVertices]();
		for(unsigned int i=0 ; i < m_mesh->mNumVertices ; i++) {
			memcpy(&(array[3*i]),&m_mesh->mTangents[i].x,3 * sizeof(GLfloat));
		}

		m_tangents.create();
		m_tangents.bind();
		m_tangents.setUsagePattern(QGLBuffer::StaticDraw);
		m_tangents.allocate(array,3 * sizeof(GLfloat) * m_mesh->mNumVertices);

		m_skinned_tangents.create();
		m_skinned_tangents.bind();
		m_skinned_tangents.setUsagePattern(QGLBuffer::DynamicDraw);
		m_skinned_tangents.allocate(array,3 * sizeof(GLfloat) * m_mesh->mNumVertices);

		delete[] array;
	}

	if(m_mesh->mBitangents != NULL) {
		GLfloat* array = new GLfloat[3 * m_mesh->mNumVertices]();
		for(unsigned int i=0 ; i < m_mesh->mNumVertices ; i++) {
			memcpy(&(array[3*i]),&m_mesh->mBitangents[i].x,3 * sizeof(GLfloat));
		}

		m_bitangents.create();
		m_bitangents.bind();
		m_bitangents.setUsagePattern(QGLBuffer::StaticDraw);
		m_bitangents.allocate(array,3 * sizeof(GLfloat) * m_mesh->mNumVertices);

		m_skinned_bitangents.create();
		m_skinned_bitangents.bind();
		m_skinned_bitangents.setUsagePattern(QGLBuffer::DynamicDraw);
		m_skinned_bitangents.allocate(array,3 * sizeof(GLfloat) * m_mesh->mNumVertices);

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
}

void AssimpMesh::draw(unsigned int submesh, QGLShaderProgram* program, int flags)
{
	if(submesh < m_submeshes.size()) {
		m_submeshes[submesh]->draw(program,flags);
	}

	debugGL("while rendering" << name());
}

void AssimpMesh::draw(unsigned int submesh, const QMap<QString, Matrix4f>& matrix_palette, QGLShaderProgram *program, int flags)
{
	if(submesh < m_submeshes.size()) {
		m_submeshes[submesh]->skin(matrix_palette);
		m_submeshes[submesh]->draw(program,flags);
	}

	debugGL("while rendering" << name());
}

void AssimpMesh::Submesh::draw(QGLShaderProgram* program, int flags)
{
	bool wireframe = flags & WIREFRAME;
	bool skinned = flags & SKINNED;

	glPushMatrix();
	m_transform.glMultf();

	if(wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if(!skinned) {
		if(!m_vertices.isCreated() || !m_indices.isCreated())
			return;
	} else {
		if(!m_skinned_vertices.isCreated() || !m_indices.isCreated())
			return;
	}

	if(m_texcoords.isCreated() && !wireframe)
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

	if(!skinned) {
		if(m_normals.isCreated() && !wireframe)
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
	} else {
		if(m_skinned_normals.isCreated() && !wireframe)
		{
			glEnable(GL_LIGHTING);
			glShadeModel(GL_SMOOTH);
			glEnableClientState(GL_NORMAL_ARRAY);
			m_skinned_normals.bind();
			glNormalPointer(GL_FLOAT, 0, NULL);
		}
		else
		{
			glDisable(GL_LIGHTING);
		}
	}

	if(program != NULL && !wireframe)
	{
		int location = program->attributeLocation("tangent");
		if(location != -1)
		{
			if(!skinned) {
				m_tangents.bind();
			} else {
				m_skinned_tangents.bind();
			}
			program->enableAttributeArray(location);
			program->setAttributeBuffer(location,GL_FLOAT,0,3);
		}
		location = program->attributeLocation("bitangent");
		if(location != -1)
		{
			if(!skinned) {
				m_bitangents.bind();
			} else {
				m_skinned_bitangents.bind();
			}
			program->enableAttributeArray(location);
			program->setAttributeBuffer(location,GL_FLOAT,0,3);
		}
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	if(!skinned) {
		m_vertices.bind();
	} else {
		m_skinned_vertices.bind();
	}
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glEnableClientState(GL_INDEX_ARRAY);
	m_indices.bind();
	glIndexPointer(GL_SHORT, 0, NULL);

	glDrawElements(GL_TRIANGLES, 3*m_nbFaces, GL_UNSIGNED_SHORT, NULL);

	m_indices.release();
	if(!skinned) {
		m_vertices.release();
	} else {
		m_skinned_vertices.release();
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);

	if(wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	glPopMatrix();
}

void AssimpMesh::Submesh::skin(const QMap<QString, Matrix4f>& matrix_palette)
{
	Matrix4f transform = (Matrix4f)m_transform;
	Matrix4f inverted_transform = transform.getInverse();

	GLfloat* array = new GLfloat[3 * m_mesh->mNumVertices]();

	if(m_mesh->mVertices != NULL) {
		for(unsigned int i=0 ; i < m_mesh->mNumVertices ; i++) {
			array[3*i] = 0.0;
			array[3*i+1] = 0.0;
			array[3*i+2] = 0.0;
		}

		for(int i=0 ; i < m_mesh->mNumBones ; i++) {
			aiBone* bone = m_mesh->mBones[i];
			QString bone_name(bone->mName.data);
			Matrix4f matrix = inverted_transform * matrix_palette.find(bone_name).value() * transform;
			for(int j=0 ; j < bone->mNumWeights ; j++) {
				aiVertexWeight* weight = &bone->mWeights[j];
				int vertexid = weight->mVertexId;

				Vector4f displacement(m_mesh->mVertices[vertexid].x,
									  m_mesh->mVertices[vertexid].y,
									  m_mesh->mVertices[vertexid].z,1);
				displacement = matrix * displacement;
				displacement = displacement * weight->mWeight;

				array[3*vertexid] += displacement.x;
				array[3*vertexid+1] += displacement.y;
				array[3*vertexid+2] += displacement.z;
			}
		}

		m_skinned_vertices.bind();
		m_skinned_vertices.write(0,array,3 * sizeof(GLfloat) * m_mesh->mNumVertices);
	}

	if(m_mesh->mNormals != NULL) {
		for(unsigned int i=0 ; i < m_mesh->mNumVertices ; i++) {
			array[3*i] = 0.0;
			array[3*i+1] = 0.0;
			array[3*i+2] = 0.0;
		}

		for(int i=0 ; i < m_mesh->mNumBones ; i++) {
			aiBone* bone = m_mesh->mBones[i];
			QString bone_name(bone->mName.data);
			Matrix4f matrix = inverted_transform * matrix_palette.find(bone_name).value() * transform;
			for(int j=0 ; j < bone->mNumWeights ; j++) {
				aiVertexWeight* weight = &bone->mWeights[j];
				int vertexid = weight->mVertexId;

				Vector4f displacement(m_mesh->mNormals[vertexid].x,
									  m_mesh->mNormals[vertexid].y,
									  m_mesh->mNormals[vertexid].z,0);
				displacement = matrix * displacement;
				displacement = displacement * weight->mWeight;

				array[3*vertexid] += displacement.x;
				array[3*vertexid+1] += displacement.y;
				array[3*vertexid+2] += displacement.z;
			}
		}

		m_skinned_normals.bind();
		m_skinned_normals.write(0, array,3 * sizeof(GLfloat) * m_mesh->mNumVertices);
	}

	if(m_mesh->mTangents != NULL) {
		for(unsigned int i=0 ; i < m_mesh->mNumVertices ; i++) {
			array[3*i] = 0.0;
			array[3*i+1] = 0.0;
			array[3*i+2] = 0.0;
		}

		for(int i=0 ; i < m_mesh->mNumBones ; i++) {
			aiBone* bone = m_mesh->mBones[i];
			QString bone_name(bone->mName.data);
			Matrix4f matrix = inverted_transform * matrix_palette.find(bone_name).value() * transform;
			for(int j=0 ; j < bone->mNumWeights ; j++) {
				aiVertexWeight* weight = &bone->mWeights[j];
				int vertexid = weight->mVertexId;

				Vector4f displacement(m_mesh->mTangents[vertexid].x,
									  m_mesh->mTangents[vertexid].y,
									  m_mesh->mTangents[vertexid].z,0);
				displacement = matrix * displacement;
				displacement = displacement * weight->mWeight;

				array[3*vertexid] += displacement.x;
				array[3*vertexid+1] += displacement.y;
				array[3*vertexid+2] += displacement.z;
			}
		}

		m_skinned_tangents.bind();
		m_skinned_tangents.write(0, array,3 * sizeof(GLfloat) * m_mesh->mNumVertices);
	}

		if(m_mesh->mBitangents != NULL) {
		for(unsigned int i=0 ; i < m_mesh->mNumVertices ; i++) {
			array[3*i] = 0.0;
			array[3*i+1] = 0.0;
			array[3*i+2] = 0.0;
		}

		for(int i=0 ; i < m_mesh->mNumBones ; i++) {
			aiBone* bone = m_mesh->mBones[i];
			QString bone_name(bone->mName.data);
			Matrix4f matrix = inverted_transform * matrix_palette.find(bone_name).value() * transform;
			for(int j=0 ; j < bone->mNumWeights ; j++) {
				aiVertexWeight* weight = &bone->mWeights[j];
				int vertexid = weight->mVertexId;

				Vector4f displacement(m_mesh->mBitangents[vertexid].x,
									  m_mesh->mBitangents[vertexid].y,
									  m_mesh->mBitangents[vertexid].z,0);
				displacement = matrix * displacement;
				displacement = displacement * weight->mWeight;

				array[3*vertexid] += displacement.x;
				array[3*vertexid+1] += displacement.y;
				array[3*vertexid+2] += displacement.z;
			}
		}

		m_skinned_bitangents.bind();
		m_skinned_bitangents.write(0, array,3 * sizeof(GLfloat) * m_mesh->mNumVertices);
	}

	delete[] array;
}

unsigned int AssimpMesh::nbSubmeshes()
{
	return m_submeshes.size();
}

Skeleton* AssimpMesh::getSkeleton()
{
	return m_skeleton;
}
