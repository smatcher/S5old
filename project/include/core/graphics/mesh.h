#ifndef MESH_H
#define MESH_H

#include "core/resources/resource.h"
#include <QtOpenGL>

class MeshData : public ResourceData
{
public:
	MeshData(const QString& name, const QString& path, IResourceFactory* factory) : ResourceData(name,path,factory) {}
	virtual void draw() = 0;
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
