#include "core/properties/meshrenderer.h"
#include "core/scenegraph/node.h"

MeshRenderer::MeshRenderer(Mesh& mesh, Material& material, Texture& texture) : IRenderable("MeshRenderer")
{
	m_mesh = mesh;
	m_material = material;
	m_texture = texture;
}

void MeshRenderer::render(double elapsed_time, GLWidget* context)
{

	if(&m_material != NULL)
		((MaterialData*)&m_material)->apply();
	else
		qDebug() << "MeshRenderer : no material to apply for " << node()->getName();

	if(&m_texture != NULL)
		((TextureData*)&m_texture)->bind();
	else
		qDebug() << "MeshRenderer : no texture to bind for " << node()->getName();

	if(&m_mesh != NULL)
		((MeshData*)&m_mesh)->draw();
	else
		qDebug() << "MeshRenderer : no mesh to draw for " << node()->getName();
}
