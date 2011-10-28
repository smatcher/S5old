#ifndef TRAIL_RENDERER_H
#define TRAIL_RENDERER_H

#include <QtOpenGL>

#include <core/properties/irenderable.h>
#include <core/graphics/material.h>
#include <core/maths/transform.h>

class TrailRenderer : public IRenderable
{
public:
	TrailRenderer(Material mat);

	void frameBegin(double elapsed_time);
	void render();

private:
	Vector4f m_lastPosition;
	Material m_material;
	QGLBuffer m_vertices;
	QGLBuffer m_normals;
	QGLBuffer m_colors;
	QGLBuffer m_texcoords;
	QGLBuffer m_indices;
	int m_nbQuads;
	int m_vertexOffset;
	int m_quadOffset;
};

#endif //TRAIL_RENDERER_H
