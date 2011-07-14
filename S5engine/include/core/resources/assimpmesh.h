#ifndef ASSIMPMESH_H
#define ASSIMPMESH_H

#include "core/graphics/mesh.h"
#include <QtOpenGL>

class aiMesh;
class AssimpFactory;
class Skeleton;

class AssimpMesh : public MeshData
{
	friend class AssimpFactory;

private:

	struct Submesh {
		const aiMesh* m_mesh;

		QGLBuffer m_vertices;
		QGLBuffer m_normals;
		QGLBuffer m_colors;
		QGLBuffer m_texcoords;
		QGLBuffer m_tangents;
		QGLBuffer m_bitangents;
		QGLBuffer m_indices;
		int m_nbFaces;

		Submesh(const aiMesh* mesh):
		m_mesh(mesh),
		m_vertices(),
		m_normals(),
		m_colors(),
		m_texcoords(),
		m_tangents(),
		m_bitangents(),
		m_indices(QGLBuffer::IndexBuffer),
		m_nbFaces(0) {}

		void buildVBO(QString name);
		void draw(QGLShaderProgram* program);
	};

	QVector<Submesh*> m_submeshes;
	Skeleton* m_skeleton;

public :
	AssimpMesh(const QString& name, const QString& path, IResourceFactory* factory);

	void buildVBOs();
	virtual bool unload();

	virtual void draw(unsigned int submesh, QGLShaderProgram* program = NULL);
	virtual unsigned int nbSubmeshes();

	virtual Skeleton* getSkeleton();
};


#endif // ASSIMPMESH_H
