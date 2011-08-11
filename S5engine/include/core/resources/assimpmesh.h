#ifndef ASSIMPMESH_H
#define ASSIMPMESH_H

#include "core/graphics/mesh.h"
#include "core/maths/transform.h"
#include <QtOpenGL>

class aiMesh;
class aiScene;
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
		m_transform(transform) {}

		void buildVBO(QString name);
		void draw(QGLShaderProgram* program, bool skinned);
		void skin(const QMap<QString, Matrix4f>& matrix_palette);
	};

	QVector<Submesh*> m_submeshes;
	Skeleton* m_skeleton;
	const aiScene* m_scene;


public :
	AssimpMesh(const QString& name, const QString& path, IResourceFactory* factory);

	void buildVBOs();
	virtual bool unload();

	virtual void draw(unsigned int submesh, QGLShaderProgram* program = NULL);
	virtual void draw(unsigned int submesh, const QMap<QString, Matrix4f>& matrix_palette, QGLShaderProgram* program = NULL);
	virtual unsigned int nbSubmeshes();

	virtual Skeleton* getSkeleton();
};


#endif // ASSIMPMESH_H
