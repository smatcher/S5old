#include <core/properties/terrainrenderer.h>
#include <core/resources/stbimage.h>

TerrainRenderer::TerrainRenderer(Texture& hm): IRenderable("TerrainRenderer") {

	/* Pas terrible, vue que la Texture n'est pas FORCEMENT une StbImage, à améliorer donc... */
	stbi_uc* image = ((StbImage*)*hm)->getData();
	int comp = ((StbImage*)*hm)->getComp();
	m_height = hm->getHeight();
	m_width = hm->getWidth();

	logInfo( "Creating terrain from " << hm->name() );

	m_heightmap = new float[m_height * m_width];

	for(int i = 0; i<m_height * m_width; i++) {
		m_heightmap[i] = ((float)image[i*comp]*20.0f)/255.0f;
	}
}

void TerrainRenderer::render(double elapsed_time, GLWidget* context) {
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

}
