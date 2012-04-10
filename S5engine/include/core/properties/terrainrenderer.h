#ifndef TERRAIN_H
#define TERRAIN_H

#include <core/properties/irenderable.h>
#include <core/graphics/texture.h>
#include <core/graphics/material.h>

#ifdef WITH_TOOLS
	#include "tools/widgets/terrainwidget.h"
#endif

#include "core/utils/quadtree.h"

#define PATCH_DIM 16
#define PATCH_CORE_VBO_SIZE (6*(PATCH_DIM-2)*(PATCH_DIM-2))
#define PATCH_HIRES_EDGE_VBO_SIZE (6*(PATCH_DIM-1))
#define PATCH_LORES_EDGE_VBO_SIZE (9*((PATCH_DIM)/2) - 6)

class TerrainPatch {
public:
	TerrainPatch(TerrainRenderer* terrain, int start_x, int start_y, int lodx, int lody,  int dim, int lod, int theight);

	void drawNormalEdge(int edge);
	void drawAdaptedEdge(int nx, int ny, int edge);

	void render();

	TerrainRenderer* m_terrain;

	QGLBuffer m_indices_core;
	QGLBuffer m_indices_edges[4][2]; /* 2 niveaux d'adaptation, 4 bords par niveau */

	/* Position du patch dans le terrain */
	int m_offsetx;
	int m_offsety;

	/* Coordonnes dans les tables de LOD */
	int m_lodx;
	int m_lody;

	/* Dimension du patch (on considère que les patchs sont carrés, parce que fuck*/
	int m_dim;

	/* Niveau de détail du patch */
	int m_lod;

	/* Sett par evalLOD si on dois dessiner ce patch */
	bool m_drawn;

	/* Debug info */
	float y_min;
	float y_max;
};

class TerrainRenderer : public IRenderable
{
	friend class TerrainWidget;

public:
	typedef QuadTree<TerrainPatch> TerrainNode;
	TerrainRenderer(Texture& hm, Material& mat, float yscale, float scale, float tscale);
	void render();

	int getMaxLod();
	bool getLodDrawn(int lod, int x, int y);
	void setLodDrawn(int lod, int x, int y, bool value);
	void printLodMap();
	bool isTransparent() {return false;}
	bool receivesShadows();
	bool castsShadows();

	int m_height;
	int m_width;

protected:

	void buildQuadTree();
	void _buildQuadTree(TerrainNode* node, int theight, int twidth, int lodx, int lody);
	void evalLOD(TerrainNode* node, const Vector3f& cameraPosition);
	void renderQuadTree(TerrainNode* node);

	float* m_heightmap;
	bool** m_lod_drawn;
	float m_yscale;
	float m_scale;
	float m_tscale;

	int m_maxlod;

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
