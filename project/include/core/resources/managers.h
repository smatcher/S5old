#ifndef MANAGERS_H
#define MANAGERS_H

#include "core/graphics/mesh.h"
#include "core/graphics/texture.h"
#include "core/graphics/material.h"
#include "core/graphics/shader.h"

#include "core/resources/resourcemanager.h"

#include "core/utils/singleton.h"

typedef Singleton< ResourceManager< MeshData,     Mesh> >     MeshManager;
typedef Singleton< ResourceManager< TextureData,  Texture> >  TextureManager;
typedef Singleton< ResourceManager< MaterialData, Material> > MaterialManager;
typedef Singleton< ResourceManager< ShaderData,   Shader> >   ShaderManager;

#endif // MANAGERS_H
