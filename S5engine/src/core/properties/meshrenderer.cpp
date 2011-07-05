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

	node()->getGlobalTransform().glMultf();

	if(!m_material.isValid())
	{
		debug( "RENDERING" , "MeshRenderer : no material to apply for " << node()->getName());
	}

	if(m_mesh.isValid())
	{
		for(unsigned int i=0 ; i<m_mesh->nbSubmeshes() ; i++) {

			if(m_material.isValid()) {
				m_material->apply(i);
				program = m_material->program(i);
			}

			m_mesh->draw(i,program);

			if(m_material.isValid()) {
				m_material->unset(i);
			}
		}
	}
	else
	{
		debug( "RENDERING" , "MeshRenderer : no mesh to draw for " << node()->getName());
	}
}

bool MeshRenderer::isTransparent()
{
	bool ret = false;

	if(m_material.isValid())
	{
		for(unsigned int i=0 ; i<m_mesh->nbSubmeshes() ; i++) {
			if(m_material->isTransparent(i)) {
				ret = true;
				break;
			}
		}
	}

	return ret;
}

#ifdef WITH_TOOLS

PropertyWidget* MeshRenderer::getWidget()
{
	if(m_widget == NULL)
		m_widget = new MeshRendererWidget(this);

	return m_widget;
}

#endif
