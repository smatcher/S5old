#include "core/resources/assimpfactory.h"
#include "core/resources/assimpmesh.h"
#include "core/resources/assimpmaterial.h"
#include <aiPostProcess.h>
#include <aiScene.h>

#include "core/resources/managers.h"

#include "debug/log/log.h"

void AssimpFactory::load(ResourceData* resource)
{
	if(resource->state() == AssimpMesh::STATE_UNLOADED)
	{
		QList<ResourceData*> content;
		parseFile(resource->path(), content);
	}
}

void AssimpFactory::parseFile(const QString& path, QList<ResourceData*>& content)
{
	const aiScene* scene = m_importer.ReadFile(path.toLocal8Bit().data(), aiProcessPreset_TargetRealtime_Quality);

	if(scene != NULL)
	{
		for(unsigned i=0 ; i<scene->mNumMeshes ; i++)
		{
			const aiMesh* aimesh = scene->mMeshes[i];
			const aiMaterial* aimaterial = scene->mMaterials[aimesh->mMaterialIndex];
			QString name = aimesh->mName.data;
			AssimpMesh* mesh = new AssimpMesh(name,path,this,aimesh);
			AssimpMaterial* material = new AssimpMaterial(name,path,this,aimaterial);
			content.push_back(mesh);
			MATERIAL_MANAGER.add(material);
			logInfo( "Assimp found the mesh : " << name << " in " << path );
		}
	}
	else
	{
		logError( "Assimp could not read " << path );
	}
}


