#include "core/resources/rawmesh.h"
#include <QtOpenGL>

RawMesh::RawMesh(const QString &name, const QString &path, IResourceFactory *factory) :
    MeshData(name,path,factory),
    m_vertices(NULL),
    m_colors(NULL),
    m_normals(NULL),
    m_texcoords(NULL),
    m_nbVertex(0),
    m_faces(NULL),
    m_nbFaces(0)
{
}

bool RawMesh::unload()
{
    if(m_vertices != NULL) {
        delete[] m_vertices;
        m_vertices = NULL;
    }

    if(m_colors != NULL) {
        delete[] m_colors;
        m_colors = NULL;
    }

    if(m_normals != NULL) {
        delete[] m_normals;
        m_colors = NULL;
    }

    if(m_texcoords != NULL) {
        delete[] m_texcoords;
        m_texcoords = NULL;
    }

    m_nbVertex = 0;

    if(m_faces != NULL) {
        for(int i = 0 ; i < m_nbFaces ; i++) {
            delete[] m_faces[i].indices;
        }

        delete[] m_faces;
        m_nbFaces = 0;
    }

    return true;
}

void RawMesh::draw()
{
    if(m_texcoords != NULL)
        glEnable(GL_TEXTURE_2D);
    else
        glDisable(GL_TEXTURE_2D);

    if(m_colors != NULL)
        glEnable(GL_COLOR_MATERIAL);
    else
        glDisable(GL_COLOR_MATERIAL);

    for(int i=0 ; i<m_nbFaces ; i++) {
        GLenum face_mode;

        switch(m_faces[i].nbIndices) {
            case 1: face_mode = GL_POINTS; break;
            case 2: face_mode = GL_LINES; break;
            case 3: face_mode = GL_TRIANGLES; break;
            default: face_mode = GL_POLYGON; break;
        }

        glBegin(face_mode);

        for(int j=0 ; j<m_faces[i].nbIndices ; j++) {
            int index = m_faces[i].indices[j];

            if(m_colors != NULL)
                glColor4bv(&m_colors[4*index]);
            if(m_normals != NULL)
                glNormal3fv(&m_normals[3*index]);
            if(m_texcoords != NULL)
                glTexCoord2fv(&m_texcoords[2*index]);
            glVertex3fv(&m_vertices[3*index]);
        }

        glEnd();
    }
};
