#ifndef MESH_H
#define MESH_H

#include "core/resources/iresource.h"
#include <QtOpenGL>

class Mesh : public IResource
{
public:

    struct Vertex
    {
        GLfloat x;
        GLfloat y;
        GLfloat z;
    };

    struct Color
    {
        GLfloat r;
        GLfloat g;
        GLfloat b;
    };

    struct UV
    {
        GLfloat u;
        GLfloat v;
    };

    struct Face
    {
        int index1;
        int index2;
        int index3;
        int index4;
    };

public:

    Mesh();
    ~Mesh();

    const Vertex* getVertices() const;
    int getNbVertices() const;

    const Vertex* getNormals() const;
    int getNbNormals() const;

    const Color* getColors() const;
    int getNbColors() const;

    const UV* getUV1() const;
    int getNbUV1() const;
    const UV* getUV2() const;
    int getNbUV2() const;

    const Face* getFaces() const;
    int getNbFaces() const;

    RenderMecanism getRenderMecanism() const;

protected:

    Vertex* m_vertices;
    int m_nb_vertices;

    Vertex* m_normals;
    int m_nb_normals;

    Color* m_colors;
    int m_nb_colors;

    UV* m_uv1;
    int m_nb_uv1;
    UV* m_uv2;
    int m_nb_uv2;

    Face* m_faces;
    int m_nb_faces;
};

#endif // MESH_H
