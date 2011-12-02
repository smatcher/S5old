#ifndef MESH_H
#define MESH_H

#include "core/resources/resource.h"
#include "core/maths/matrix4.h"
#include "core/maths/boundingvolume.h"

#ifdef WITH_TOOLS
	#include "tools/widgets/meshwidget.h"
#endif

#include <QtOpenGL>
#include <QMap>

class Mesh;
class Skeleton;
template <class R, class H> class ResourceManager;

class MeshData : public ResourceData
{
	friend class ResourceHandle<MeshData>;
	friend class ResourceManager<MeshData,Mesh>;
	#ifdef WITH_TOOLS
		friend class MeshWidget;
	#endif

public:

	enum DrawFlags
	{
		NONE = 0x0,
		WIREFRAME = 0x1,
		SKINNED = 0x2
	};

	MeshData(const QString& name, const QString& path, IResourceFactory* factory) : ResourceData(name,path,factory) {}
	virtual void draw(unsigned int submesh, int flags = NONE) = 0;
	virtual void draw(unsigned int submesh, const QMap<QString, Matrix4f>& matrix_palette, int flags = NONE) = 0;
	virtual unsigned int nbSubmeshes() = 0;

	virtual Skeleton* getSkeleton() {return NULL;}
	virtual const BoundingVolume* getBoundingVolume() = 0;
	virtual int getNbVertices() = 0;

#ifdef WITH_TOOLS
	virtual void drawPreview() = 0;

	virtual ResourceWidget* getWidget()
	{
		if(!m_widget)
			m_widget = new MeshWidget(*this);

		return m_widget;
	}
#endif
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
