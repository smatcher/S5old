#ifndef MANAGERS_H
#define MANAGERS_H

#include "core/graphics/mesh.h"
#include "core/graphics/texture.h"
#include "core/graphics/material.h"
#include "core/graphics/shader.h"
#include "core/resources/sample.h"

#include "core/resources/resourcemanager.h"

#include "core/utils/singleton.h"

#include "core/resources/assimpfactory.h"
#include "core/resources/stbimage.h"
#include "core/resources/samplefactory.h"

#include "core/utils/primitives.h"

typedef Singleton< ResourceManager< MeshData,     Mesh> >     SingletonMeshManager;
typedef Singleton< ResourceManager< TextureData,  Texture> >  SingletonTextureManager;
typedef Singleton< ResourceManager< MaterialData, Material> > SingletonMaterialManager;
typedef Singleton< ResourceManager< ShaderData,   Shader> >   SingletonShaderManager;
typedef Singleton< ResourceManager< SampleData,   Sample> >   SingletonSampleManager;

#define MESH_MANAGER SingletonMeshManager::getInstance()
#define TEXTURE_MANAGER SingletonTextureManager::getInstance()
#define MATERIAL_MANAGER SingletonMaterialManager::getInstance()
#define SHADER_MANAGER SingletonShaderManager::getInstance()
#define SAMPLE_MANAGER SingletonSampleManager::getInstance()

inline void initResourceManagers()
{
	MESH_MANAGER.addFactory(new AssimpFactory()); // TODO : this is a memory leak, i don't like leaks, get rid of this leak.
	TEXTURE_MANAGER.addFactory(new StbImageFactory());
    SAMPLE_MANAGER.addFactory(new SampleFactory());

    MESH_MANAGER.parseDir("../media/models",true);
	TEXTURE_MANAGER.parseDir("../media/textures",true);
    TEXTURE_MANAGER.loadAll();
    SAMPLE_MANAGER.parseDir("../media/sounds",true);
    SAMPLE_MANAGER.loadAll();

    MESH_MANAGER.add(PrimitiveMesh::buildPlane());
}

#endif // MANAGERS_H
