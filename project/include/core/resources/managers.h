#ifndef MANAGERS_H
#define MANAGERS_H

#include "core/graphics/mesh.h"
#include "core/graphics/texture.h"
#include "core/graphics/material.h"
#include "core/graphics/shader.h"

#include "core/resources/resourcemanager.h"

#include "core/utils/singleton.h"

#include "core/resources/assimpfactory.h"
#include "core/resources/stbimage.h"

typedef Singleton< ResourceManager< MeshData,     Mesh> >     MeshManager;
typedef Singleton< ResourceManager< TextureData,  Texture> >  TextureManager;
typedef Singleton< ResourceManager< MaterialData, Material> > MaterialManager;
typedef Singleton< ResourceManager< ShaderData,   Shader> >   ShaderManager;

inline void initResourceManagers()
{
	MeshManager::getInstance().addFactory(new AssimpFactory()); // TODO : this is a memory leak, i don't like leaks, get rid of this leak.
	TextureManager::getInstance().addFactory(new StbImageFactory());

	MeshManager::getInstance().parseDir("../media/models",true);
	TextureManager::getInstance().parseDir("../media/textures",true);
	TextureManager::getInstance().loadAll();
}

#endif // MANAGERS_H
