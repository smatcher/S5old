#ifndef ASSIMPMESH_H
#define ASSIMPMESH_H

#include "core/graphics/mesh.h"
#include <QtOpenGL>

class aiMesh;
class AssimpMesh : public MeshData
{
private:
	const aiMesh* m_mesh;

	QGLBuffer m_vertices;
	QGLBuffer m_normals;
	QGLBuffer m_colors;
	QGLBuffer m_texcoords;
	QGLBuffer m_indices;
	int m_nbFaces;

public :
	AssimpMesh(const QString& name, const QString& path, IResourceFactory* factory, const aiMesh* mesh);

	void buildVBO();
	virtual bool unload();

	virtual void draw();
};


#endif // ASSIMPMESH_H
