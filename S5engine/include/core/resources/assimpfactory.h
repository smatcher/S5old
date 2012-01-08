#ifndef ASSIMPFACTORY_H
#define ASSIMPFACTORY_H

#include "core/resources/iresourcefactory.h"
#include "core/graphics/mesh.h"
#include "core/maths/transform.h"
#include <assimp.hpp>
#include "core/resources/assimpmesh.h"

class aiNode;
class Skeleton;

class AssimpFactory : public IResourceFactory
{
public:
	virtual void load(ResourceData* resource);

protected:
	virtual void parseFile(const QString& path, QList<ResourceData*>& content, const QHash<QString,QString>& rules);

private:
	Assimp::Importer m_importer;

	Skeleton* buildSkeleton(aiNode* node);

	QString findMeshName(unsigned int meshNumber, aiNode* node);
	aiNode* findBone(QString name, aiNode* node);
	void buildSubMeshes(QVector<AssimpMesh::Submesh*>& submeshes, const aiNode* node, const aiScene* scene, QVector<QString> wanted = QVector<QString>(), Transformf parent_transform = Transformf());

	void parseMetaFile(QString meta_path, QString path, QList<ResourceData*>& content);
};

#endif // ASSIMPFACTORY_H
