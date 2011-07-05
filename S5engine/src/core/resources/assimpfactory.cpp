#include "core/resources/assimpfactory.h"
#include "core/resources/assimpmesh.h"
#include <aiPostProcess.h>
#include <aiScene.h>

#include "core/resources/managers.h"

#include "core/log/log.h"

#include <QVector>

#include <core/managers/physicsmanager.h>

void AssimpFactory::load(ResourceData* resource)
{
	AssimpMesh* meshresource = static_cast<AssimpMesh*>(resource);

	if(resource->state() == AssimpMesh::STATE_UNLOADED)
	{
		const aiScene* scene = m_importer.ReadFile(resource->path().toLocal8Bit().data(), aiProcessPreset_TargetRealtime_Quality);

		if(scene != NULL)
		{
			debug("ASSIMPLOADER",scene->mNumAnimations << "animations");

			unsigned int numVertices = 0;
			for(unsigned i=0 ; i<scene->mNumMeshes ; i++)
			{
				const aiMesh* aimesh = scene->mMeshes[i];
				numVertices += aimesh->mNumVertices;
				meshresource->m_submeshes.push_back(new AssimpMesh::Submesh(aimesh));
			}

			GLfloat* array = new GLfloat[3 * numVertices]();
			for(unsigned int i=0 ; i<meshresource->m_submeshes.size() ; i++) {
				for(unsigned int j=0 ; j < meshresource->m_submeshes[i]->m_mesh->mNumVertices ; j++) {
					memcpy(&(array[3*j]),&meshresource->m_submeshes[i]->m_mesh->mVertices[j].x,3 * sizeof(GLfloat));
				}
			}
			PHYSICS_MANAGER.buildConvexCollider(resource->name(),array,numVertices);

			meshresource->buildVBOs();
			meshresource->m_state = ResourceData::STATE_LOADED;
		}
		else
		{
			logError( "Assimp could not read " << resource->path());
		}
	}
}

void AssimpFactory::parseFile(const QString& path, QList<ResourceData*>& content, const QHash<QString,QString>&)
{
	QDir dir(path);
	QString name = dir.dirName();

	// Chop extension
	int id = name.lastIndexOf(".");
	if(id > 1)
	{
		QString extension = name.right(id).toLower();
		if(extension != "x"
		&& extension != "blend"
		&& extension != "dae") {
			name.chop(name.length() - id);
		} else {
			return; // Not an assimp model
		}
	} else {
		return; // No extension
	}

	dir.cdUp();
	if(dir.exists())
	{
		debug( "RESOURCE PARSING" , "AssimpMesh found " << name);
		AssimpMesh* mesh = new AssimpMesh(name,path,this);
		content.push_back(mesh);
	}
	else
	{
		debug( "RESOURCE PARSING" , path << " : " << dir << " does not exist");
	}
}

QString AssimpFactory::findMeshName(unsigned int meshNumber, aiNode* node)
{
	for(unsigned int i=0 ; i < node->mNumMeshes ; i++) {
		if(node->mMeshes[i] == meshNumber) {
			return QString(node->mName.data);
		}
	}

	for(unsigned int i=0 ; i < node->mNumChildren ; i++) {
		QString ret = findMeshName(meshNumber, node->mChildren[i]);
		if(ret.length() > 0) {
			return ret;
		}
	}

	return "";
}
