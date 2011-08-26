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

			#ifdef WITH_TOOLS
				if(node() != NULL && node()->isSelected()) {
					glClearStencil(0);
					glClear( GL_STENCIL_BUFFER_BIT );
					glEnable( GL_STENCIL_TEST );
					// Set the stencil buffer to write a 1 in every time
					// a pixel is written to the screen

					glStencilFunc( GL_ALWAYS, 1, 0xFFFF );
					glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
				 }
			#endif

			if(m_material.isValid()) {
				m_material->apply(i);
				program = m_material->program(i);
			}

			m_mesh->draw(i,program);

			if(m_material.isValid()) {
				m_material->unset(i);
			}

			#ifdef WITH_TOOLS
				if(node() != NULL && node()->isSelected()) {
					// Set the stencil buffer to only allow writing
					// to the screen when the value of the
					// stencil buffer is not 1
					glStencilFunc( GL_NOTEQUAL, 1, 0xFFFF );
					glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
					// Draw the object with thick lines
					context->qglColor(Qt::white);
					glLineWidth(3.0);

					m_mesh->draw(i,program, true);

					glLineWidth(1.0);
					glDisable(GL_STENCIL_TEST);
				}
			#endif
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
