#ifndef MESH_H
#define MESH_H

#include "core/resources/resource.h"
#include <QtOpenGL>

class Mesh;
class Skeleton;
template <class R, class H> class ResourceManager;

class MeshData : public ResourceData
{
	friend class ResourceHandle<MeshData>;
	friend class ResourceManager<MeshData,Mesh>;

public:
	MeshData(const QString& name, const QString& path, IResourceFactory* factory) : ResourceData(name,path,factory) {}
	virtual void draw(unsigned int submesh, QGLShaderProgram* program = NULL) = 0;
	virtual unsigned int nbSubmeshes() = 0;

	virtual Skeleton* getSkeleton() {return NULL;}
};

class Mesh : public ResourceHandle<MeshData>
{
public:
	Mesh() {}
	Mesh(const Mesh& from) : ResourceHandle<MeshData>(from) {}
	Mesh(MeshData& from) : ResourceHandle<MeshData>(from) {}
	virtual ~Mesh() {}
};

#endif // MESH_H
