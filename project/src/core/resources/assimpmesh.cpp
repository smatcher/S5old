#include "core/resources/assimpmesh.h"
#include <QtOpenGL>

#include <assimp.h>
#include <aiMesh.h>

AssimpMesh::AssimpMesh(const QString& name, const QString& path, IResourceFactory* factory, const aiMesh* mesh) :
	MeshData(name,path,factory),
	m_mesh(mesh)
{
}

bool AssimpMesh::unload()
{
	return false;
}

void Color4f(const struct aiColor4D *color)
{
   glColor4f(color->r, color->g, color->b, color->a);
}

void AssimpMesh::draw()
{
	if(m_mesh->mNormals == NULL) {
		glDisable(GL_LIGHTING);
	} else {
		glEnable(GL_LIGHTING);
	}

	if(m_mesh->mColors[0] != NULL) {
		glEnable(GL_COLOR_MATERIAL);
	} else {
		glDisable(GL_COLOR_MATERIAL);
	}

	if(m_mesh->mTextureCoords[0] != NULL) {
		glEnable(GL_TEXTURE_2D);
	} else {
		glDisable(GL_TEXTURE_2D);
	}

	for (unsigned t = 0; t < m_mesh->mNumFaces; ++t) {
		const struct aiFace* face = &m_mesh->mFaces[t];
		GLenum face_mode;

		switch(face->mNumIndices) {
			case 1: face_mode = GL_POINTS; break;
			case 2: face_mode = GL_LINES; break;
			case 3: face_mode = GL_TRIANGLES; break;
			default: face_mode = GL_POLYGON; break;
		}

		glBegin(face_mode);

		for(unsigned i = 0; i < face->mNumIndices; i++) {
			int index = face->mIndices[i];
			if(m_mesh->mColors[0] != NULL)
			Color4f(&m_mesh->mColors[0][index]);
			if(m_mesh->mNormals != NULL)
			glNormal3fv(&m_mesh->mNormals[index].x);
			if(m_mesh->mTextureCoords[0] != NULL)
			glTexCoord2fv(&m_mesh->mTextureCoords[0][index].x);
			glVertex3fv(&m_mesh->mVertices[index].x);
		}

		glEnd();
	}
}
