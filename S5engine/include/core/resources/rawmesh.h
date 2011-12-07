#ifndef RAWMESH_H
#define RAWMESH_H

#include <QtOpenGL>
#include "core/graphics/mesh.h"

class RawMesh : public MeshData
{
public:

	RawMesh(const QString& name, const QString& path, IResourceFactory* factory);
	virtual bool unload();

	virtual void draw(unsigned int submesh, int flags = NONE);
	virtual void draw(unsigned int submesh, const QMap<QString, Matrix4f>& matrix_palette, int flags = NONE);
	virtual unsigned int nbSubmeshes();

	virtual const BoundingVolume* getBoundingVolume();
	virtual int getNbVertices();
	virtual int getNbFaces();

	#ifdef WITH_TOOLS
		virtual void drawPreview();
	#endif

protected:

	QGLBuffer m_vertices;
	QGLBuffer m_normals;
	QGLBuffer m_colors;
	QGLBuffer m_texcoords;
	QGLBuffer m_tangents;
	QGLBuffer m_bitangents;
	QGLBuffer m_indices;
	int m_nbFaces;
	int m_nbVertices;

	BoundingVolume* m_boundingVolume;
};

#endif // RAWMESH_H
