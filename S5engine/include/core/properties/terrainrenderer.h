#ifndef TERRAIN_H
#define TERRAIN_H

#include <core/properties/irenderable.h>
#include <core/graphics/texture.h>
#include <core/graphics/material.h>

#ifdef WITH_TOOLS
	#include "tools/widgets/terrainwidget.h"
#endif


class TerrainRenderer : public IRenderable
{
	friend class TerrainWidget;
public:
	TerrainRenderer(Texture& hm, Material& mat, float yscale, float scale, float tscale);
	void render(double elapsed_time, GLWidget* context);
	bool isTransparent() {return false;}

protected:
	float* m_heightmap;
	int m_height;
	int m_width;
	float m_yscale;
	float m_scale;
	float m_tscale;

	bool m_wireframe;

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

#ifdef WITH_TOOLS
	virtual PropertyWidget* getWidget();
#endif
};

#endif // TERRAIN_H
