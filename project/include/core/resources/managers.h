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
#include "core/resources/xmlmaterial.h"
#include "core/resources/glslshader.h"

#include "core/utils/primitives.h"

#define MEDIA_DIR "media/"

typedef Singleton< ResourceManager< MeshData,           Mesh> >           SingletonMeshManager;
typedef Singleton< ResourceManager< TextureData,        Texture> >        SingletonTextureManager;
typedef Singleton< ResourceManager< MaterialData,       Material> >       SingletonMaterialManager;
typedef Singleton< ResourceManager< SampleData,         Sample> >         SingletonSampleManager;
typedef Singleton< ResourceManager< ShaderProgramData,  ShaderProgram> >  SingletonShaderProgramManager;
typedef Singleton< ResourceManager< ShaderData,         Shader> >         SingletonShaderManager;

#define MESH_MANAGER SingletonMeshManager::getInstance()
#define TEXTURE_MANAGER SingletonTextureManager::getInstance()
#define MATERIAL_MANAGER SingletonMaterialManager::getInstance()
#define SHADER_PROGRAM_MANAGER SingletonShaderProgramManager::getInstance()
#define SHADER_MANAGER SingletonShaderManager::getInstance()
#define SAMPLE_MANAGER SingletonSampleManager::getInstance()

inline void initResourceManagers()
{
	MESH_MANAGER.addFactory(new AssimpFactory()); // TODO : this is a memory leak, i don't like leaks, get rid of this leak.
	TEXTURE_MANAGER.addFactory(new StbImageFactory());
	SAMPLE_MANAGER.addFactory(new SampleFactory());
	SHADER_MANAGER.addFactory(new GLSLShaderFactory());
	SHADER_PROGRAM_MANAGER.addFactory(new GLSLShaderProgramFactory());
	MATERIAL_MANAGER.addFactory(new XmlMaterialFactory());

	QString base_dir = MEDIA_DIR;

	MESH_MANAGER.parseDir(base_dir + "models",true);
	MESH_MANAGER.loadAll();
	TEXTURE_MANAGER.parseDir(base_dir + "textures",true);
	TEXTURE_MANAGER.loadAll();
	SAMPLE_MANAGER.parseDir(base_dir + "sounds",true);
	SAMPLE_MANAGER.loadAll();
	SHADER_MANAGER.parseDir(base_dir + "shaders",true);
	SHADER_MANAGER.loadAll();
	SHADER_PROGRAM_MANAGER.parseDir(base_dir + "shaders",true);
	SHADER_PROGRAM_MANAGER.loadAll();
	MATERIAL_MANAGER.parseDir(base_dir + "materials",true);
	MATERIAL_MANAGER.loadAll();

	MESH_MANAGER.add(PrimitiveMesh::buildCube());
	MESH_MANAGER.add(PrimitiveMesh::buildPlane());
	MESH_MANAGER.add(PrimitiveMesh::buildSphere(16,32));
}

#endif // MANAGERS_H
