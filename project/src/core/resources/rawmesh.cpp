#include <GL/glew.h>
#include <GL/gl.h>
#include "core/resources/rawmesh.h"

RawMesh::RawMesh(const QString &name, const QString &path, IResourceFactory *factory) :
	MeshData(name,path,factory),
	m_vertices(0),
	m_colors(0),
	m_normals(0),
	m_texcoords(0),
	m_indices(0),
	m_nbFaces(0)
{
}

bool RawMesh::unload()
{
	if(m_vertices != 0) {
		glDeleteBuffers(1,&m_vertices);
		m_vertices = 0;
	}
	if(m_colors != 0) {
		glDeleteBuffers(1,&m_colors);
	}
	/* TODO
	m_colors.destroy();
	m_texcoords.destroy();
	m_normals.destroy();
	m_indices.destroy();
*/
	m_nbFaces = 0;

	return true;
}

void RawMesh::draw()
{
	if(m_vertices == 0 || m_indices == 0)
		return;

	if(m_texcoords != 0)
	{
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, m_texcoords);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	if(m_colors != 0)
	{
		glEnable(GL_COLOR_MATERIAL);
		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, m_colors);
		glColorPointer(4, GL_FLOAT, 0, NULL);
	}
	else
	{
		glDisable(GL_COLOR_MATERIAL);
	}

	if(m_normals != 0)
	{
		glEnable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, m_normals);
		glNormalPointer(GL_FLOAT, 0, NULL);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glEnableClientState(GL_INDEX_ARRAY);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);
	glIndexPointer(GL_SHORT, 0, NULL);

	glDrawElements(GL_TRIANGLES, m_nbFaces, GL_UNSIGNED_SHORT, NULL);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);

	debugGL("while rendering" << name());
};
