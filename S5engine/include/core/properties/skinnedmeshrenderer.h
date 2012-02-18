#ifndef SKINNEDMESHRENDERER_H
#define SKINNEDMESHRENDERER_H

#include "core/properties/irenderable.h"
#include "core/graphics/mesh.h"
#include "core/graphics/material.h"
#include "core/animation/skeleton.h"

#include <QMap>
#include <QPair>

class SkinnedMeshRenderer : public IRenderable
{
public:
	/// Constructor
	SkinnedMeshRenderer(Mesh& mesh, Material& material);
	/// Destructor
	virtual ~SkinnedMeshRenderer();

	virtual void render();
	bool isTransparent();
	virtual bool usesSSS();
	bool receivesShadows();
	bool castsShadows();

	void createLinks();

private:
	Mesh     m_mesh;
	Material m_material;
	QMap<QString, QPair<Skeleton::Bone*, Node*> > m_links;

	void generateMatrixPalette(Skeleton::Bone* bone, QMap<QString,Matrix4f>& matrix_palette, const Matrix4f& inverted_renderer_transform);
	void linkBoneToNode(Skeleton::Bone* bone, Node* node);
};

#endif // SKINNEDMESHRENDERER_H
