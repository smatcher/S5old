#include "core/resources/assimpmesh.h"

#include "core/maths/aabb.h"
#include "core/managers/rendermanager.h"

#include <assimp.h>
#include <aiMesh.h>

#include <GL/glu.h>

//#ifdef _WIN32
	void __ClientActiveTexture(int texid);
//#endif

AssimpMesh::AssimpMesh(const QString& name, const QString& path, IResourceFactory* factory) :
	MeshData(name,path,factory),
	m_skeleton(0),
	m_nbVertices(0),
	m_nbFaces(0),
	m_boundingVolume(0)
{
	m_state = STATE_UNLOADED;
	m_insidelib = false;
}


bool AssimpMesh::unload()
{
	return false;
}

void AssimpMesh::buildVBOs()
{
	m_nbVertices = 0;
	m_nbFaces = 0;
	for(int i=0 ; i<m_submeshes.size() ; i++) {
		m_submeshes[i]->buildVBO(name());
		m_nbVertices += m_submeshes[i]->m_mesh->mNumVertices;
		m_nbFaces += m_submeshes[i]->m_nbFaces;
	}
}

void AssimpMesh::Submesh::buildAABB(float& minX,float& maxX, float& minY, float& maxY, float& minZ, float& maxZ)
{

	if(m_mesh->mNumVertices > 0) {
		minX = m_mesh->mVertices[0].x;
		maxX = m_mesh->mVertices[0].x;
		minY = m_mesh->mVertices[0].y;
		maxY = m_mesh->mVertices[0].y;
		minZ = m_mesh->mVertices[0].z;
		maxZ = m_mesh->mVertices[0].z;

		for(int i=1 ; i<m_mesh->mNumVertices ; i++) {
			if(m_mesh->mVertices[i].x < minX)
				minX = m_mesh->mVertices[i].x;
			else if(m_mesh->mVertices[i].x > maxX)
				maxX = m_mesh->mVertices[i].x;
			if(m_mesh->mVertices[i].y < minY)
				minY = m_mesh->mVertices[i].y;
			else if(m_mesh->mVertices[i].y > maxY)
				maxY = m_mesh->mVertices[i].y;
			if(m_mesh->mVertices[i].z < minZ)
				minZ = m_mesh->mVertices[i].z;
			else if(m_mesh->mVertices[i].z > maxZ)
				maxZ = m_mesh->mVertices[i].z;
		}

	} else {
		minX = 0;
		maxX = 0;
		minY = 0;
		maxY = 0;
		minZ = 0;
		maxZ = 0;
	}

	Vector3f min(minX,minY,minZ);
	Vector3f max(maxX,maxY,maxZ);

	min = m_transform.apply(min);
	max = m_transform.apply(max);
}

void AssimpMesh::buildAABB()
{
	float minX, maxX, minY, maxY, minZ, maxZ;
	for(int i=0 ; i<m_submeshes.size() ; i++) {
		float _minX, _maxX, _minY, _maxY, _minZ, _maxZ;
		m_submeshes[i]->buildAABB(_minX,_maxX,_minY,_maxY,_minZ,_maxZ);
		if(i==0) {
			minX = _minX;
			maxX = _maxX;
			minY = _minY;
			maxY = _maxY;
			minZ = _minZ;
			maxZ = _maxZ;
		} else {
			if(_minX < minX) minX = _minX;
			if(_maxX > maxX) maxX = _maxX;
			if(_minY < minY) minY = _minY;
			if(_maxY > maxY) maxY = _maxY;
			if(_minZ < minZ) minZ = _minZ;
			if(_maxZ > maxZ) maxZ = _maxZ;
		}
	}

	if(m_boundingVolume)
		delete m_boundingVolume;

	Vector3f min(minX,minY,minZ);
	Vector3f max(maxX,maxY,maxZ);
	Vector3f center = (max+min)/2;
	Vector3f halfdim = (max-min)/2;
	m_boundingVolume = new AABB(center,halfdim);
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

	if(m_mesh->mVertices != 0) {
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

	if(m_mesh->mNormals != 0) {
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

	if(m_mesh->mColors[0] != 0) {
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

	if(m_mesh->mTextureCoords[0] != 0) {
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

	if(m_mesh->mTangents != 0) {
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

	if(m_mesh->mBitangents != 0) {
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

void AssimpMesh::draw(unsigned int submesh, int flags)
{
	QGLShaderProgram* program = RENDER_MANAGER.getRenderPassInfo()->ubershader_used->program();

	if(submesh < m_submeshes.size()) {
		m_submeshes[submesh]->draw(program,flags);
	}

	debugGL("while rendering" << name());
}

void AssimpMesh::draw(unsigned int submesh, const QMap<QString, Matrix4f>& matrix_palette, int flags)
{
	QGLShaderProgram* program = RENDER_MANAGER.getRenderPassInfo()->ubershader_used->program();

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
		__ClientActiveTexture(GL_TEXTURE0);
		m_texcoords.bind();
		glTexCoordPointer(2, GL_FLOAT, 0, 0);
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
		glColorPointer(4, GL_FLOAT, 0, 0);
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
			glNormalPointer(GL_FLOAT, 0, 0);
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
			glNormalPointer(GL_FLOAT, 0, 0);
		}
		else
		{
			glDisable(GL_LIGHTING);
		}
	}

	if(program != 0 && !wireframe)
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
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glEnableClientState(GL_INDEX_ARRAY);
	m_indices.bind();
	glIndexPointer(GL_SHORT, 0, 0);

	glDrawElements(GL_TRIANGLES, 3*m_nbFaces, GL_UNSIGNED_SHORT, 0);

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

	if(m_mesh->mVertices != 0) {
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

	if(m_mesh->mNormals != 0) {
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

	if(m_mesh->mTangents != 0) {
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

		if(m_mesh->mBitangents != 0) {
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

int AssimpMesh::getNbVertices()
{
	return m_nbVertices;
}

int AssimpMesh::getNbFaces()
{
	return m_nbFaces;
}

const BoundingVolume* AssimpMesh::getBoundingVolume()
{
	return m_boundingVolume;
}

Skeleton* AssimpMesh::getSkeleton()
{
	return m_skeleton;
}

void AssimpMesh::setInsideLib(bool inside)
{
	m_insidelib = inside;
}

bool AssimpMesh::isInsideLib()
{
	return m_insidelib;
}

void AssimpMesh::addSubmeshNode(QString node)
{
	m_file_nodes.push_back(node);
}

const QVector<QString>& AssimpMesh::getSubmeshNodes()
{
	return m_file_nodes;
}

#ifdef WITH_TOOLS
void AssimpMesh::drawPreview()
{
	bool wireframe = false;//flags & WIREFRAME;

	if(wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	for(int i=0 ; i<nbSubmeshes() ; i++) {
		glPushMatrix();
		m_submeshes[i]->m_transform.glMultf();

		if(!m_submeshes[i]->m_vertices.isCreated() || !m_submeshes[i]->m_indices.isCreated())
			return;

		if(m_submeshes[i]->m_normals.isCreated() && !wireframe)
		{
			glEnable(GL_LIGHTING);
			glShadeModel(GL_SMOOTH);
			glEnableClientState(GL_NORMAL_ARRAY);
			m_submeshes[i]->m_normals.bind();
			glNormalPointer(GL_FLOAT, 0, 0);
		}

		glEnableClientState(GL_VERTEX_ARRAY);
		m_submeshes[i]->m_vertices.bind();
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glEnableClientState(GL_INDEX_ARRAY);
		m_submeshes[i]->m_indices.bind();
		glIndexPointer(GL_SHORT, 0, 0);

		glDrawElements(GL_TRIANGLES, 3*m_submeshes[i]->m_nbFaces, GL_UNSIGNED_SHORT, 0);

		m_submeshes[i]->m_indices.release();
		m_submeshes[i]->m_vertices.release();

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_INDEX_ARRAY);
		glPopMatrix();
	}

	if(wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	debugGL("while rendering" << name() << "preview");
}
#endif
