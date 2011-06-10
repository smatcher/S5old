#ifndef TERRAIN_H
#define TERRAIN_H

#include <core/properties/irenderable.h>
#include <core/graphics/texture.h>

class TerrainRenderer : public IRenderable
{
public:
	TerrainRenderer(Texture& hm, float vscale);
	void render(double elapsed_time, GLWidget* context);
	bool isTransparent() {return false;}

private:
	float* m_heightmap;
	int m_height;
	int m_width;

	/* VBO Data */
	QGLBuffer m_vertices;
	QGLBuffer m_normals;
	QGLBuffer m_colors;
	QGLBuffer m_texcoords;
	QGLBuffer m_tangents;
	QGLBuffer m_bitangents;
	QGLBuffer m_indices;
	int m_nbFaces;
};

#endif // TERRAIN_H
