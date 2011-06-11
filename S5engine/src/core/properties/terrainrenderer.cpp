#include <core/properties/terrainrenderer.h>
#include <core/resources/stbimage.h>

#include "core/scenegraph/node.h"

#include <QtOpenGL>

TerrainRenderer::TerrainRenderer(Texture& hm, float vscale): IRenderable("TerrainRenderer"), m_indices(QGLBuffer::IndexBuffer) {

	/* Pas terrible, vue que la Texture n'est pas FORCEMENT une StbImage, à améliorer donc... */
	stbi_uc* image = ((StbImage*)*hm)->getData();
	int comp = ((StbImage*)*hm)->getComp();

	GLfloat* vertices;
	GLfloat* normals;
	GLint* indices;
	int index;

	m_height = hm->getHeight();
	m_width = hm->getWidth();

	logInfo( "Creating terrain from " << hm->name() );

	m_heightmap = new float[m_height * m_width];

	for(int i = 0; i<m_height * m_width; i++) {
		m_heightmap[i] = ((float)image[i*comp]*20.0f)/255.0f;
	}

	/**** VERTICES ****/
	vertices = new GLfloat[m_height * m_width * 3]();
	for(int x = 0; x<m_height; x++) {;
		for(int z = 0; z<m_width; z++) {
			index = (x + z*m_width);
			vertices[index*3] = (float) x;
			vertices[index*3 + 1] = ((float)image[index*comp]*vscale)/255.0f;
			vertices[index*3 + 2] = (float) z;
		}
	}
	m_vertices.create();
	m_vertices.bind();
	m_vertices.setUsagePattern(QGLBuffer::StaticDraw);
	m_vertices.allocate(vertices, m_height * m_width * 3 * sizeof(GLfloat));

	/**** NORMALS ****/

	/**** TANGENT ****/

	/**** BITENGENT ****/

	/**** TEXCOORD ****/

	/**** INDICES ****/
	indices = new GLint[3*((m_height-1)*(m_width-1)*2)]();
	for(int x = 0; x<m_height-1; x++) {
		for(int z = 0; z<m_width-1; z++) {
			index = (x + z*m_width)*6;

			/* Triangle 1 */
			indices[index]	 = (x+z*m_width);
			indices[index+1] = ((x+1)+z*m_width);
			indices[index+2] = (x+(z+1)*m_width);
			/* Triangle 2 */
			indices[index+3] = (x+(z+1)*m_width);
			indices[index+4] = ((x+1)+z*m_width);
			indices[index+5] = ((x+1)+(z+1)*m_width);
		}
	}
	m_indices.create();
	m_indices.bind();
	m_indices.setUsagePattern(QGLBuffer::StaticDraw);
	m_indices.allocate(indices, 3*((m_height-1)*(m_width-1)*2)*sizeof(GLint));

	m_vertices.release();
	m_indices.release();

	delete[] indices;

}

void TerrainRenderer::render(double elapsed_time, GLWidget* context) {

	node()->getGlobalTransform().glMultf();

	glBegin(GL_LINES);
	for(int x = 0; x<m_height; x++) {;
		for(int y = 0; y<m_width; y++) {
			if(y<m_width-1) {
				glVertex3f((float)x,m_heightmap[x + y*m_width],(float)y);
				glVertex3f((float)x,m_heightmap[x + (y+1)*m_width],(float)y+1.0f);
			}
			if(x<m_height-1) {
				glVertex3f((float)x,m_heightmap[x + y*m_width],(float)y);
				glVertex3f((float)x + 1.0f,m_heightmap[x + 1 + y*m_width],(float)y);
			}
		}
	}
	glEnd();

	if(!m_vertices.isCreated() || !m_indices.isCreated()) {
		return;
	}

	if(m_colors.isCreated())
	{
		glEnable(GL_COLOR_MATERIAL);
		glEnableClientState(GL_COLOR_ARRAY);
		m_colors.bind();
		glColorPointer(4, GL_FLOAT, 0, NULL);
	}
	else
	{
		glDisable(GL_COLOR_MATERIAL);
	}

	if(m_normals.isCreated())
	{
		glEnable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		glEnableClientState(GL_NORMAL_ARRAY);
		m_normals.bind();
		glNormalPointer(GL_FLOAT, 0, NULL);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	m_vertices.bind();
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glEnableClientState(GL_INDEX_ARRAY);
	m_indices.bind();
	glIndexPointer(GL_INT, 0, NULL);

	glDrawElements(GL_TRIANGLES, 3*((m_height-1)*(m_width-1)*2), GL_UNSIGNED_INT, NULL);

	m_indices.release();
	m_vertices.release();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
}
