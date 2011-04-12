#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "core/properties/irenderable.h"

#include <aiMesh.h>
#include <aiMaterial.h>

//class Mesh;
//class Material;

class MeshRenderer : public IRenderable
{
public:
	MeshRenderer(aiMesh* mesh, aiMaterial* material);
	void render(double elapsed_time, GLWidget* context);

private:
	aiMesh* m_mesh;
	aiMaterial* m_material;
	//Mesh*     m_mesh;
	//Material* m_material;
};

#endif // MESHRENDERER_H
