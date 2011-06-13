#ifndef TERRAIN_H
#define TERRAIN_H

#include <core/properties/irenderable.h>
#include <core/graphics/texture.h>
#include <core/graphics/material.h>

class TerrainRenderer : public IRenderable
{
public:
	TerrainRenderer(Texture& hm, Material& mat, float yscale, float scale, float tscale);
	void render(double elapsed_time, GLWidget* context);
	bool isTransparent() {return false;}

private:
	float* m_heightmap;
	int m_height;
	int m_width;
	float m_yscale;
	float m_scale;
	float m_tscale;

	Material m_material;

	/* VBO Data */
	QGLBuffer m_vertices;
	QGLBuffer m_normals;
	QGLBuffer m_colors;
	QGLBuffer m_texcoords;
	QGLBuffer m_stexcoords;
	QGLBuffer m_tangents;
	QGLBuffer m_bitangents;
	QGLBuffer m_indices;
	int m_nbFaces;
};

#endif // TERRAIN_H
