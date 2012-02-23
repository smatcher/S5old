#ifndef ASSIMPMESH_H
#define ASSIMPMESH_H

#include "core/graphics/mesh.h"
#include "core/maths/transform.h"
#include <QtOpenGL>
#include <QGLFunctions>

class aiMesh;
class aiScene;
class AssimpFactory;
class Skeleton;

class AssimpMesh : public MeshData
{
	friend class AssimpFactory;

private:

	struct Submesh : protected QGLFunctions {
		const aiMesh* m_mesh;

		QGLBuffer m_vertices;
		QGLBuffer m_normals;
		QGLBuffer m_colors;
		QGLBuffer m_texcoords;
		QGLBuffer m_tangents;
		QGLBuffer m_bitangents;
		QGLBuffer m_indices;

		QGLBuffer m_skinned_vertices;
		QGLBuffer m_skinned_normals;
		QGLBuffer m_skinned_tangents;
		QGLBuffer m_skinned_bitangents;

		int m_nbFaces;
		Transformf m_transform;

		Submesh(const aiMesh* mesh, const Transformf& transform):
		m_mesh(mesh),
		m_vertices(),
		m_normals(),
		m_colors(),
		m_texcoords(),
		m_tangents(),
		m_bitangents(),
		m_indices(QGLBuffer::IndexBuffer),
		m_skinned_vertices(),
		m_skinned_normals(),
		m_skinned_tangents(),
		m_skinned_bitangents(),
		m_nbFaces(0),
		m_transform(transform)
		{
			initializeGLFunctions();
		}

		void buildVBO(QString name);
		void buildAABB(float& minX,float& maxX, float& minY, float& maxY, float& minZ, float& maxZ);
		void draw(QGLShaderProgram* program, int flags);
		void skin(const QMap<QString, Matrix4f>& matrix_palette);
	};

	QVector<Submesh*> m_submeshes;
	Skeleton* m_skeleton;
	int m_nbVertices;
	int m_nbFaces;
	BoundingVolume* m_boundingVolume;
	const aiScene* m_scene;

	// Loading infos
	QVector<QString> m_file_nodes;
	bool m_insidelib;

public :
	AssimpMesh(const QString& name, const QString& path, IResourceFactory* factory);

	void buildVBOs();
	void buildAABB();
	virtual bool unload();

	virtual void draw(unsigned int submesh, int flags = NONE);
	virtual void draw(unsigned int submesh, const QMap<QString, Matrix4f>& matrix_palette, int flags = NONE);
	virtual unsigned int nbSubmeshes();

	virtual const BoundingVolume* getBoundingVolume();
	virtual int getNbVertices();
	virtual int getNbFaces();

	#ifdef WITH_TOOLS
		virtual void drawPreview();
	#endif

	virtual Skeleton* getSkeleton();

	void setInsideLib(bool inside);
	bool isInsideLib();
	void addSubmeshNode(QString node);
	const QVector<QString>& getSubmeshNodes();
};


#endif // ASSIMPMESH_H
