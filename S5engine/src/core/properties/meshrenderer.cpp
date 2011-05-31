#include "core/properties/meshrenderer.h"
#include "core/scenegraph/node.h"
#include "tools/widgets/meshrendererwidget.h"

#include "core/log/log.h"

#include <QtOpenGL>

MeshRenderer::MeshRenderer(Mesh& mesh, Material& material) : IRenderable("MeshRenderer")
{
	m_mesh = mesh;
	m_material = material;
}

void MeshRenderer::render(double elapsed_time, GLWidget* context)
{
	// The program is passed to the mesh in order to set the attributes
	QGLShaderProgram* program = NULL;

	node()->globalTransform().glMultf();

	if(m_material.isValid())
	{
		m_material->apply();
		program = m_material->program();
	}
	else
	{
		debug( "RENDERING" , "MeshRenderer : no material to apply for " << node()->getName());
	}

	if(m_mesh.isValid())
	{
		m_mesh->draw(program);
	}
	else
	{
		debug( "RENDERING" , "MeshRenderer : no mesh to draw for " << node()->getName());
	}

	if(m_material.isValid())
	{
		m_material->unset();
	}
}

#ifdef WITH_TOOLS

PropertyWidget* MeshRenderer::getWidget()
{
	if(m_widget == NULL)
		m_widget = new MeshRendererWidget(this);

	return m_widget;
}

#endif
