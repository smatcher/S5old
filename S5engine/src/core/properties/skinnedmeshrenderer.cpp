#include "core/properties/skinnedmeshrenderer.h"
#include "core/scenegraph/node.h"

#include "core/log/log.h"

#include <QtOpenGL>

SkinnedMeshRenderer::SkinnedMeshRenderer(Mesh& mesh, Material& material) : IRenderable("SkinnedMeshRenderer")
{
	m_mesh = mesh;
	m_material = material;
}

SkinnedMeshRenderer::~SkinnedMeshRenderer()
{
}

void SkinnedMeshRenderer::render(double elapsed_time, GLWidget *context)
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
		// Generate matrix palette
		QMap<QString,Matrix4f> matrix_palette;
		Matrix4f inverted_root_transform = node()->getGlobalTransform().getInverse(); // Should use root bone instead of node()

		generateMatrixPalette(m_mesh->getSkeleton()->getRootBone(),matrix_palette, inverted_root_transform);

		// Render
		for(unsigned int i=0 ; i<m_mesh->nbSubmeshes() ; i++) {

			if(m_material.isValid()) {
				m_material->apply(i);
				program = m_material->program(i);
			}

			m_mesh->draw(i,matrix_palette,program);

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

bool SkinnedMeshRenderer::isTransparent()
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

void SkinnedMeshRenderer::linkBoneToNode(Skeleton::Bone* bone, Node* node)
{
	for(int i=0 ; i<node->childCount() ; i++) {
		Node* child = node->child(i);

		if(child->getName() == bone->m_name) {
			QPair<Skeleton::Bone*, Node*> pair(bone, child);
			m_links.insert(child->getName(), pair);

			for(int j=0 ; j<bone->m_childrens.count() ; j++) {
				linkBoneToNode(&bone->m_childrens[j], child);
			}
		}
	}
}

void SkinnedMeshRenderer::createLinks()
{
	if(m_mesh.isValid())
		linkBoneToNode(m_mesh->getSkeleton()->getRootBone(), node());
}

void SkinnedMeshRenderer::generateMatrixPalette(Skeleton::Bone* bone, QMap<QString,Matrix4f>& matrix_palette, const Matrix4f& inverted_root_transform)
{
	Node* node = m_links.find(bone->m_name).value().second;

	Matrix4f pose_wrt_root = inverted_root_transform * node->getGlobalTransform();

	matrix_palette.insert(bone->m_name, pose_wrt_root * bone->m_inverted_global_pose);

	for(QVector<Skeleton::Bone>::iterator it = bone->m_childrens.begin() ; it != bone->m_childrens.end() ; it++) {
		generateMatrixPalette(&(*it), matrix_palette, inverted_root_transform);
	}
}
