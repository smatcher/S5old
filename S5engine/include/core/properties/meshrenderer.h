#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "core/properties/irenderable.h"
#include "core/graphics/mesh.h"
#include "core/graphics/material.h"
#include "core/graphics/texture.h"

class MeshRendererWidget;

class MeshRenderer : public IRenderable
{
	friend class MeshRendererWidget;

public:
	MeshRenderer(Mesh& mesh, Material& material);
	void render(double elapsed_time, GLWidget* context);
	bool isTransparent();

	#ifdef WITH_TOOLS
		virtual PropertyWidget* getWidget();
	#endif
private:
	Mesh     m_mesh;
	Material m_material;
};

#endif // MESHRENDERER_H
