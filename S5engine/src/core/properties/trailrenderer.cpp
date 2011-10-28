#include <core/properties/trailrenderer.h>
#include <core/scenegraph/node.h>

#define TRAIL_MAX_QUADS 50

TrailRenderer::TrailRenderer(Material material) :
	m_material(material),
	m_vertices(),
	m_normals(),
	m_colors(),
	m_texcoords(),
	m_indices(QGLBuffer::IndexBuffer),
	m_nbQuads(-1),
	m_vertexOffset(-1),
	m_quadOffset(-1)
{
	const int nb_vertices = (TRAIL_MAX_QUADS+1)*2;
	const int nb_faces = TRAIL_MAX_QUADS;

	GLfloat* texcoords = new GLfloat[nb_vertices*2];
	for(int i=0 ; i<nb_vertices ; i++) {
		texcoords[i*2] = i/2;         // U : repeat texture
		texcoords[i*2 + 1] = (1+i)%2; // V : 1up 0down
	}
	m_texcoords.create();
	m_texcoords.bind();
	m_texcoords.setUsagePattern(QGLBuffer::StaticDraw);
	m_texcoords.allocate(texcoords,sizeof(GLfloat) * nb_vertices * 2);

	GLfloat* vertices = new GLfloat[nb_vertices*3];
	m_vertices.create();
	m_vertices.bind();
	m_vertices.setUsagePattern(QGLBuffer::DynamicDraw);
	m_vertices.allocate(vertices,sizeof(GLfloat) * nb_vertices * 3);

	GLshort* indices = new GLshort[nb_faces*6];
	for(int i=0 ; i<nb_faces ; i++) {
		indices[6*i  ] = 2*i;
		indices[6*i+1] = 2*i + 1;
		indices[6*i+2] = 2*i + 2;
		indices[6*i+3] = 2*i + 2;
		indices[6*i+4] = 2*i + 1;
		indices[6*i+5] = 2*i + 3;
	}
	m_indices.create();
	m_indices.bind();
	m_indices.setUsagePattern(QGLBuffer::StaticDraw);
	m_indices.allocate(indices,sizeof(GLshort) * nb_faces * 6);

	m_vertices.release();
	m_indices.release();

	delete[] vertices;
	delete[] indices;
}

void TrailRenderer::frameBegin(double elapsed_time)
{
	if(!m_vertices.isCreated() || !m_indices.isCreated())
		return;

	Matrix4f thisTransform = node()->getGlobalTransform();
	Vector4f thisPosition = thisTransform*Vector4f(0,0,0,1);

	if((thisPosition - m_lastPosition).norm() > 0.3) {
		m_lastPosition = thisPosition;

		Vector4f up = thisTransform*Vector4f(0,0.5,0,1);
		Vector4f dn = thisTransform*Vector4f(0,-0.5,0,1);

		// Add those in the VBO
		if(m_nbQuads < TRAIL_MAX_QUADS) {
			m_nbQuads++;
		}
		int oldVertexOffset = m_vertexOffset;
		m_vertexOffset = (m_vertexOffset+1)%(TRAIL_MAX_QUADS+1);
		m_quadOffset = (m_quadOffset+1)%(TRAIL_MAX_QUADS);
		m_vertices.bind();
		GLfloat vertex[6] = {up[0],up[1],up[2],dn[0],dn[1],dn[2]};
		m_vertices.write(m_vertexOffset*6*sizeof(GLfloat),vertex,6*sizeof(GLfloat));
		if(m_nbQuads >= TRAIL_MAX_QUADS) {
			GLshort indices[6] = {
									2*oldVertexOffset,2*oldVertexOffset+1,2*m_vertexOffset,
									2*m_vertexOffset,2*oldVertexOffset+1,2*m_vertexOffset+1
								 };
			m_indices.bind();
			m_indices.write(m_quadOffset*6*sizeof(GLshort),indices,6*sizeof(GLshort));
		}
	}
}

void TrailRenderer::render()
{
	if(!m_vertices.isCreated() || !m_indices.isCreated())
		return;

	if(m_nbQuads > 0) {

		if(m_material.isValid())
		{
			m_material->apply(0);
	//		program = m_material->program();
		}
		else
		{
			debug( "RENDERING" , "TrailRenderer : no material to apply for " << node()->getName());
		}

		if(m_texcoords.isCreated())
		{
			glEnable(GL_TEXTURE_2D);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			m_texcoords.bind();
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
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
			glColor3f(1,1,1);
		}

		glEnableClientState(GL_VERTEX_ARRAY);
		m_vertices.bind();
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glEnableClientState(GL_INDEX_ARRAY);
		m_indices.bind();
		glIndexPointer(GL_SHORT, 0, NULL);

		glDrawElements(GL_TRIANGLES, 6*m_nbQuads, GL_UNSIGNED_SHORT, NULL);

		m_indices.release();
		m_vertices.release();

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_INDEX_ARRAY);

		debugGL("while rendering TrailRenderer on " << node()->getName());

		if(m_material.isValid())
		{
			m_material->unset(0);
		}
	}
}

