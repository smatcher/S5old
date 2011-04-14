#ifndef ASSIMPMESH_H
#define ASSIMPMESH_H

#include "core/graphics/mesh.h"

class aiMesh;
class AssimpMesh : public MeshData
{
private:
	const aiMesh* m_mesh;

public :
	AssimpMesh(const QString& name, const QString& path, IResourceFactory* factory, const aiMesh* mesh);
	virtual bool unload();

	virtual void draw();
};


#endif // ASSIMPMESH_H
