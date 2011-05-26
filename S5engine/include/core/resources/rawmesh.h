#ifndef RAWMESH_H
#define RAWMESH_H

#include <QtOpenGL>
#include "core/graphics/mesh.h"

class RawMesh : public MeshData
{
public:

	RawMesh(const QString& name, const QString& path, IResourceFactory* factory);
	virtual bool unload();

	virtual void draw();

protected:

	QGLBuffer m_vertices;
	QGLBuffer m_normals;
	QGLBuffer m_colors;
	QGLBuffer m_texcoords;
	QGLBuffer m_indices;
	int m_nbFaces;
};

#endif // RAWMESH_H
