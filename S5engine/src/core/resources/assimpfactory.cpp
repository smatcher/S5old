#include "core/resources/assimpfactory.h"
#include "core/resources/assimpmesh.h"
#include <aiPostProcess.h>
#include <aiScene.h>

#include "core/resources/managers.h"

#include "core/log/log.h"

#include "core/animation/skeleton.h"

#include <QVector>
#include <QMap>

#include <core/managers/physicsmanager.h>

void AssimpFactory::load(ResourceData* resource)
{
	AssimpMesh* meshresource = static_cast<AssimpMesh*>(resource);

	if(resource->state() == AssimpMesh::STATE_UNLOADED)
	{
		const aiScene* scene = m_importer.ReadFile(resource->path().toLocal8Bit().data(), aiProcessPreset_TargetRealtime_Quality);
		QMap<QString,aiNode*> bones;

		if(scene != NULL)
		{
			debug("ASSIMPLOADER",scene->mNumAnimations << "animations");
			for(int i=0 ; i< scene->mNumAnimations ; i++) {
				debug("ASSIMPLOADER",scene->mAnimations[i]->mName.data << "animations" << scene->mAnimations[i]->mNumChannels << " channels");
				for(int j=0 ; j<scene->mAnimations[i]->mNumChannels ; j++) {
					debug("ASSIMPLOADER",scene->mAnimations[i]->mChannels[j]->mNodeName.data <<
						  scene->mAnimations[i]->mChannels[j]->mNumPositionKeys <<
						  scene->mAnimations[i]->mChannels[j]->mNumScalingKeys <<
						  scene->mAnimations[i]->mChannels[j]->mNumRotationKeys
						  );
				}
			}

			unsigned int numVertices = 0;
			for(unsigned i=0 ; i<scene->mNumMeshes ; i++)
			{
				const aiMesh* aimesh = scene->mMeshes[i];
				debug("ASSIMPLOADER",aimesh->mNumBones << "bones for submesh" << i);
				for(int j=0 ; j<aimesh->mNumBones ; j++) {
					QString bone_name = aimesh->mBones[j]->mName.data;
					debug("ASSIMPLOADER", bone_name);
					if(!bones.contains(bone_name)) {
						aiNode* bone = findBone(bone_name, scene->mRootNode);
						if(bone != NULL) {
							bones.insert(bone_name, findBone(bone_name, bone));
							debug("ASSIMPLOADER", "inserted");
						}
					}
				}
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

			if(bones.size() != 0) {
				meshresource->m_skeleton = buildSkeleton(scene->mRootNode);
			}

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

aiNode* AssimpFactory::findBone(QString name, aiNode* node)
{
	if(node->mName.data == name) {
		return node;
	}

	for(unsigned int i=0 ; i < node->mNumChildren ; i++) {
		aiNode* ret = findBone(name, node->mChildren[i]);
		if(ret != NULL) {
			return ret;
		}
	}

	return NULL;
}

void buildBone(Skeleton::Bone* bone, aiNode* node)
{
	bone->m_name = node->mName.data;
	aiVector3D scaling;
	aiQuaternion rotation;
	aiVector3D position;
	node->mTransformation.Decompose(scaling, rotation, position);
	aiMatrix3x3 rotmat = rotation.GetMatrix();

	Matrix3f _rotation;
	_rotation[0] = rotmat.a1;
	_rotation[1] = rotmat.a2;
	_rotation[2] = rotmat.a3;
	_rotation[3] = rotmat.b1;
	_rotation[4] = rotmat.b2;
	_rotation[5] = rotmat.b3;
	_rotation[6] = rotmat.c1;
	_rotation[7] = rotmat.c2;
	_rotation[8] = rotmat.c3;
	Vector3f _position(position.x, position.y, position.z);
	Vector3f _scaling(scaling.x, scaling.y, scaling.z);

	bone->m_bind_pose = Transformf(_rotation, _position, _scaling);

	for(int i=0 ; i< node->mNumChildren ; i++) {
		Skeleton::Bone b;
		buildBone(&b,node->mChildren[i]);
		bone->m_childrens.push_back(b);
	}
}

Skeleton* AssimpFactory::buildSkeleton(aiNode* node)
{
	Skeleton* ret = new Skeleton();
	buildBone(&ret->m_root_bone,node);
	return ret;
}
