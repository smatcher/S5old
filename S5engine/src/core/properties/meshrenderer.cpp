#include "core/properties/meshrenderer.h"
#include "core/scenegraph/node.h"
#include "tools/widgets/meshrendererwidget.h"

#include "core/log/log.h"

MeshRenderer::MeshRenderer(Mesh& mesh, Material& material) : IRenderable("MeshRenderer")
{
	m_mesh = mesh;
	m_material = material;
}

void MeshRenderer::render(double elapsed_time, GLWidget* context)
{
	node()->globalTransform().glMultf();

	if(m_material.isValid())
		m_material->apply();
	else
		debug( "RENDERING" , "MeshRenderer : no material to apply for " << node()->getName());

	if(m_mesh.isValid())
		m_mesh->draw();
	else
		debug( "RENDERING" , "MeshRenderer : no mesh to draw for " << node()->getName());

	if(m_material.isValid())
		m_material->unset();
}

#ifdef WITH_TOOLS

PropertyWidget* MeshRenderer::getWidget()
{
	if(m_widget == NULL)
		m_widget = new MeshRendererWidget(this);

	return m_widget;
}

#endif
