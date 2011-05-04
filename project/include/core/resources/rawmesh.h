#ifndef RAWMESH_H
#define RAWMESH_H

#include "core/graphics/mesh.h"
#include <QtOpenGL>

class RawMesh : public MeshData
{
public:

	bool m_NormalsArePerVertex;

	struct Face
	{
		int* indices;
		int nbIndices;
	};

	float* m_vertices;
	GLbyte* m_colors;
	float* m_normals;
	float* m_texcoords;
	int    m_nbVertex;

	Face* m_faces;
	int   m_nbFaces;

	RawMesh(const QString& name, const QString& path, IResourceFactory* factory);
	virtual bool unload();

	virtual void draw();
};

#endif // RAWMESH_H
