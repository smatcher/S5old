#ifndef RAWMESH_H
#define RAWMESH_H

#include <GL/gl.h>
#include "core/graphics/mesh.h"

class RawMesh : public MeshData
{
public:

	RawMesh(const QString& name, const QString& path, IResourceFactory* factory);
	virtual bool unload();

	virtual void draw();

protected:

	GLuint m_vertices;
	GLuint m_normals;
	GLuint m_colors;
	GLuint m_texcoords;
	GLuint m_indices;
	int m_nbFaces;
};

#endif // RAWMESH_H
