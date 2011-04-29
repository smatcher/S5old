#include "core/properties/meshrenderer.h"
#include "core/scenegraph/node.h"
#include "debug/widgets/meshrendererwidget.h"

#include "debug/log/log.h"

MeshRenderer::MeshRenderer(Mesh& mesh, Material& material, Texture& texture) : IRenderable("MeshRenderer")
{
	m_mesh = mesh;
	m_material = material;
	m_texture = texture;
}

void MeshRenderer::render(double elapsed_time, GLWidget* context)
{
	node()->globalTransform().glMultf();

	if(&m_material != NULL)
		((MaterialData*)&m_material)->apply();
	else
		debug( "RENDERING" , "MeshRenderer : no material to apply for " << node()->getName());

	if(&m_texture != NULL)
		((TextureData*)&m_texture)->bind();
	else
		debug( "RENDERING" , "MeshRenderer : no texture to bind for " << node()->getName());

	if(&m_mesh != NULL)
		((MeshData*)&m_mesh)->draw();
	else
		debug( "RENDERING" , "MeshRenderer : no mesh to draw for " << node()->getName());
}

#ifdef WITH_TOOLS

PropertyWidget* MeshRenderer::getWidget()
{
	if(m_widget == NULL)
		m_widget = new MeshRendererWidget(this);

	return m_widget;
}

#endif
