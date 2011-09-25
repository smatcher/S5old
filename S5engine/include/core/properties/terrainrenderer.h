#ifndef TERRAIN_H
#define TERRAIN_H

#include <core/properties/irenderable.h>
#include <core/graphics/texture.h>
#include <core/graphics/material.h>

#ifdef WITH_TOOLS
	#include "tools/widgets/terrainwidget.h"
#endif

#include "core/utils/quadtree.h"

class TerrainPatch {
public:
	TerrainPatch(int start_x, int start_y, int dim, int lod, int theight, int twidth);
	void render();

	QGLBuffer m_indices;

	/* Position du patch dans le terrain */
	int m_offsetx;
	int m_offsety;

	/* Dimension du path (on concidère que les patchs sont carrés, parce que fuck*/
	int m_dim;

	/* Niveau de détail du patch */
	int m_lod;
};

class TerrainRenderer : public IRenderable
{
	friend class TerrainWidget;

public:
	typedef QuadTree<TerrainPatch> TerrainNode;
	TerrainRenderer(Texture& hm, Material& mat, float yscale, float scale, float tscale);
	void render(GLWidget* context);
	bool isTransparent() {return false;}


protected:

	void buildQuadTree(int max_lod);
	void _buildQuadTree(TerrainNode* node, int theight, int twidth);
	void renderQuadTree(TerrainNode* node);

	float* m_heightmap;
	int m_height;
	int m_width;
	float m_yscale;
	float m_scale;
	float m_tscale;

	bool m_wireframe;

	TerrainNode* m_quadtree;

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
