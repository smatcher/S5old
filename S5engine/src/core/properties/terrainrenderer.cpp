#include <core/properties/terrainrenderer.h>
#include <core/resources/stbimage.h>

#include "core/scenegraph/node.h"

#include <QtOpenGL>

TerrainRenderer::TerrainRenderer(Texture& hm, float yscale, float scale): IRenderable("TerrainRenderer"), m_indices(QGLBuffer::IndexBuffer) {

	/* Pas terrible, vue que la Texture n'est pas FORCEMENT une StbImage, à améliorer donc... */
	stbi_uc* image = ((StbImage*)*hm)->getData();
	int comp = ((StbImage*)*hm)->getComp();

	GLfloat* vertices;
	GLfloat* normals;
	GLint* indices;
	int index;

	m_height = hm->getHeight();
	m_width = hm->getWidth();
	m_yscale = yscale;
	m_scale = scale;

	logInfo( "Creating terrain from " << hm->name() );
/*
	m_heightmap = new float[m_height * m_width];

	for(int i = 0; i<m_height * m_width; i++) {
		m_heightmap[i] = ((float)image[i*comp]*20.0f)/255.0f;
	}
*/
	/**** VERTICES ****/
	vertices = new GLfloat[m_height * m_width * 3]();
	for(int x = 0; x<m_height; x++) {;
		for(int z = 0; z<m_width; z++) {
			index = (x + z*m_height);
			vertices[index*3] = (float) x * m_scale;
			vertices[index*3 + 1] = ((float)image[index*comp]*m_yscale)/255.0f;
			vertices[index*3 + 2] = (float) z * m_scale;
		}
	}
	m_vertices.create();
	m_vertices.bind();
	m_vertices.setUsagePattern(QGLBuffer::StaticDraw);
	m_vertices.allocate(vertices, m_height * m_width * 3 * sizeof(GLfloat));

	/**** NORMALS ****/
	normals = new GLfloat[m_height * m_width * 3]();
	for(int x = 0; x<m_height; x++) {;
		for(int z = 0; z<m_width; z++) {

			index = (x + z*m_height);
			Vector3f normal(0,0,0);

			/*
			 *      B
			 *      |
			 *  A---O---C
			 *      |
			 *      D
			 */

			// OB ^ OA
			//vertices[(x+1+z*m_height)*3]
			if(x>0 && z<m_height-1) {
				normal += Vector3f(vertices[(x+(z+1)*m_height)*3] - vertices[(x+z*m_height)*3],
						   vertices[(x+(z+1)*m_height)*3 + 1] - vertices[(x+z*m_height)*3 + 1],
						   vertices[(x+(z+1)*m_height)*3 + 2] - vertices[(x+z*m_height)*3 + 2]) ^
					  Vector3f(vertices[(x-1+z*m_height)*3] - vertices[(x+z*m_height)*3],
						   vertices[(x-1+z*m_height)*3 + 1] - vertices[(x+z*m_height)*3 + 1],
						   vertices[(x-1+z*m_height)*3 + 2] - vertices[(x+z*m_height)*3 + 2]);
			}

			// OC ^ OB
			if(x<m_width-1 && z<m_height-1) {
				normal += Vector3f(vertices[(x+1+z*m_height)*3] - vertices[(x+z*m_height)*3],
						   vertices[(x+1+z*m_height)*3 + 1] - vertices[(x+z*m_height)*3 + 1],
						   vertices[(x+1+z*m_height)*3 + 2] - vertices[(x+z*m_height)*3 + 2]) ^
					  Vector3f(vertices[(x+(z+1)*m_height)*3] - vertices[(x+z*m_height)*3],
						   vertices[(x+(z+1)*m_height)*3 + 1] - vertices[(x+z*m_height)*3 + 1],
						   vertices[(x+(z+1)*m_height)*3 + 2] - vertices[(x+z*m_height)*3 + 2]);
			}
			// OD ^ OC
			if(x<m_width-1 && z>0) {
				normal += Vector3f(vertices[(x+(z-1)*m_height)*3] - vertices[(x+z*m_height)*3],
						   vertices[(x+(z-1)*m_height)*3 + 1] - vertices[(x+z*m_height)*3 + 1],
						   vertices[(x+(z-1)*m_height)*3 + 2] - vertices[(x+z*m_height)*3 + 2]) ^
					  Vector3f(vertices[(x+1+z*m_height)*3] - vertices[(x+z*m_height)*3],
						   vertices[(x+1+z*m_height)*3 + 1] - vertices[(x+z*m_height)*3 + 1],
						   vertices[(x+1+z*m_height)*3 + 2] - vertices[(x+z*m_height)*3 + 2]);
			}
			// OA ^ OD
			if(x>0 && z>0) {
				normal += Vector3f(vertices[(x-1+z*m_height)*3] - vertices[(x+z*m_height)*3],
						   vertices[(x-1+z*m_height)*3 + 1] - vertices[(x+z*m_height)*3 + 1],
						   vertices[(x-1+z*m_height)*3 + 2] - vertices[(x+z*m_height)*3 + 2]) ^
					  Vector3f(vertices[(x+(z-1)*m_height)*3] - vertices[(x+z*m_height)*3],
						   vertices[(x+(z-1)*m_height)*3 + 1] - vertices[(x+z*m_height)*3 + 1],
						   vertices[(x+(z-1)*m_height)*3 + 2] - vertices[(x+z*m_height)*3 + 2]);
			}

			normal *= -1.0f; /* Ouais j'ai calculé les normales à l'envers, a corriger quand j'aurais pas la fleme */
			normal.normalize();

			normals[index*3] = normal.x;
			normals[index*3 + 1] = normal.y;
			normals[index*3 + 2] = normal.z;
		}
	}
	m_normals.create();
	m_normals.bind();
	m_normals.setUsagePattern(QGLBuffer::StaticDraw);
	m_normals.allocate(normals, m_height * m_width * 3 * sizeof(GLfloat));

	/**** TANGENT ****/

	/**** BITENGENT ****/

	/**** TEXCOORD ****/

	/**** INDICES ****/
	indices = new GLint[3*((m_height-1)*(m_width-1)*2)]();
	for(int x = 0; x<m_height-1; x++) {
		for(int z = 0; z<m_width-1; z++) {
			index = (x + z*m_height)*6;

			/* Triangle 1 */
			indices[index]	 = (x+z*m_height);
			indices[index+1] = (x+(z+1)*m_height);
			indices[index+2] = ((x+1)+z*m_height);
			/* Triangle 2 */
			indices[index+3] = (x+(z+1)*m_height);
			indices[index+4] = ((x+1)+(z+1)*m_height);
			indices[index+5] = ((x+1)+z*m_height);
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
