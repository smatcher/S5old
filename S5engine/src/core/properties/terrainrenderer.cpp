#include <core/properties/terrainrenderer.h>
#include <core/resources/stbimage.h>

#include <core/managers/rendermanager.h>

#include <core/properties/iproperty.h>

#include "core/scenegraph/node.h"
#include "core/utils/quadtree.h"

#include <QtOpenGL>

//#ifdef _WIN32
	void __ClientActiveTexture(int);
//#endif

TerrainPatch::TerrainPatch(int start_x, int start_y, int dim, int lod, int theight, int twidth) :
	m_indices(QGLBuffer::IndexBuffer),
	m_offsetx(start_x),
	m_offsety(start_y),
	m_dim(dim),
	m_lod(lod) {

	//logInfo("Creating terrain patch with LOD="<<lod<<" at ("<<start_x<<","<<start_y<<")");

	/* On saute 2^lod vertex*/
	int increment = 1<<m_lod;
	int x;
	int nx;
	int y;
	int ny;
	int index = 0;
	GLint indices[6*((m_dim/increment)*(m_dim/increment))];

	for(x=0; x<m_dim/increment; x++) {
		for(y=0; y<m_dim/increment; y++) {
			nx = x*increment + start_x;
			ny = y*increment + start_y;
			/* Triangle 1 */
			indices[index]	 = (nx+ny*theight);
			indices[index+1] = (nx+(ny+increment)*theight);
			indices[index+2] = ((nx+increment)+ny*theight);
			/* Triangle 2 */
			indices[index+3] = (nx+(ny+increment)*theight);
			indices[index+4] = ((nx+increment)+(ny+increment)*theight);
			indices[index+5] = ((nx+increment)+ny*theight);
			index += 6;
		}
	}

	m_indices.create();
	m_indices.bind();
	m_indices.setUsagePattern(QGLBuffer::StaticDraw);
	m_indices.allocate(indices, 6*((m_dim/increment)*(m_dim/increment))*sizeof(GLint));

	m_indices.release();
}

void TerrainPatch::render() {
	int increment = 1<<m_lod;
	glEnableClientState(GL_INDEX_ARRAY);
	m_indices.bind();
	glIndexPointer(GL_INT, 0, 0);
	glDrawElements(GL_TRIANGLES, 6*((m_dim/increment)*(m_dim/increment)), GL_UNSIGNED_INT, 0);
	m_indices.release();
}

TerrainRenderer::TerrainRenderer(Texture& hm, Material& mat, float yscale, float scale, float tscale): IRenderable("TerrainRenderer"),
	m_indices(QGLBuffer::IndexBuffer),
	m_height(hm->getHeight()),
	m_width(hm->getWidth()),
	m_yscale(yscale),
	m_scale(scale),
	m_tscale(tscale),
	m_material(mat),
	m_wireframe(false){

	logInfo( "Creating terrain from " << hm->name() );

	/* Pas terrible, vue que la Texture n'est pas FORCEMENT une StbImage,   améliorer donc... */
	stbi_uc* image = ((StbImage*)*hm)->getData();

	GLfloat* vertices;
	GLfloat* normals;
	GLfloat* texcoords;
	GLfloat* stexcoords;
	GLint* indices;

	int index;
	int comp = ((StbImage*)*hm)->getComp();

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

			normal *= -1.0f; /* Ouais j'ai calculé les normales   l'envers, a corriger quand j'aurais pas la fleme */
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
	texcoords = new GLfloat[m_height * m_width * 2]();
	for(int x = 0; x<m_height; x++) {
		for(int z = 0; z<m_width; z++) {
			index = (x + z*m_height)*2;

			texcoords[index]     = ((float)x)/m_tscale; // u
			texcoords[index + 1] = ((float)z)/m_tscale; // v
		}
	}
	m_texcoords.create();
	m_texcoords.bind();
	m_texcoords.setUsagePattern(QGLBuffer::StaticDraw);
	m_texcoords.allocate(texcoords, m_height * m_width * 2 * sizeof(float));

	/**** STEXCOORD ****/
	stexcoords = new GLfloat[m_height * m_width * 2]();

	for(int x = 0; x<m_height; x++) {
		for(int z = 0; z<m_width; z++) {
			index = (x + z*m_height)*2;

			stexcoords[index]     = (float)x/(float)m_height; // u
			stexcoords[index + 1] = (float)z/(float)m_width; // v
		}
	}
	m_stexcoords.create();
	m_stexcoords.bind();
	m_stexcoords.setUsagePattern(QGLBuffer::StaticDraw);
	m_stexcoords.allocate(stexcoords, m_height * m_width * 2 * sizeof(float));

	m_stexcoords.release();

    buildQuadTree(8);

}

void TerrainRenderer::render() {   
    static Node* cam_node = 0;
    IProperty* cam = (IProperty*)RENDER_MANAGER.getCurrentCamera();
    if(cam!=0)
        cam_node = cam->node();

	node()->getGlobalTransform().glMultf();

    if(!m_vertices.isCreated() /*|| !m_indices.isCreated()*/) {
		return;
	}

	/* Application du Material */
	if(m_material.isValid())
	{
		m_material->apply(0);
	}
	else
	{
		debug( "RENDERING" , "TerrainRenderer : no material to apply for " << node()->getName());
	}

	if(m_texcoords.isCreated() && m_stexcoords.isCreated())
	{
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		__ClientActiveTexture(GL_TEXTURE0);
		m_texcoords.bind();
		glTexCoordPointer(2, GL_FLOAT, 0, 0);

		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		__ClientActiveTexture(GL_TEXTURE1);
		m_stexcoords.bind();
		glTexCoordPointer(2, GL_FLOAT, 0, 0);
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
		glColorPointer(4, GL_FLOAT, 0, 0);
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
		glNormalPointer(GL_FLOAT, 0, 0);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	m_vertices.bind();
	glVertexPointer(3, GL_FLOAT, 0, 0);

	if(m_wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	//m_quadtree->getValue()->render();
    renderQuadTree(m_quadtree, cam_node);

	if(m_wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	m_vertices.release();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);

	/* Désactivation du material */
	m_material->unset(0);
}

void TerrainRenderer::buildQuadTree(int max_lod) {
	m_quadtree = new TerrainNode(new TerrainPatch(0,0,m_height,max_lod,m_height,m_width), 0, TerrainNode::ROOT);
	_buildQuadTree(m_quadtree, m_height, m_width);
}

void TerrainRenderer::_buildQuadTree(TerrainNode* node, int theight, int twidth) {
	if(node->getValue()->m_lod > 0) {
		node->addChild(TerrainNode::NORTH_WEST, new TerrainPatch(node->getValue()->m_offsetx,
								 node->getValue()->m_offsety + node->getValue()->m_dim/2,
								 node->getValue()->m_dim/2,
								 node->getValue()->m_lod-1,
								 theight, twidth));
		node->addChild(TerrainNode::SOUTH_WEST, new TerrainPatch(node->getValue()->m_offsetx,
								 node->getValue()->m_offsety,
								 node->getValue()->m_dim/2,
								 node->getValue()->m_lod-1,
								 theight, twidth));
		node->addChild(TerrainNode::NORTH_EAST, new TerrainPatch(node->getValue()->m_offsetx + node->getValue()->m_dim/2,
								 node->getValue()->m_offsety + node->getValue()->m_dim/2,
								 node->getValue()->m_dim/2,
								 node->getValue()->m_lod-1,
								 theight, twidth));
		node->addChild(TerrainNode::SOUTH_EAST, new TerrainPatch(node->getValue()->m_offsetx  + node->getValue()->m_dim/2,
								 node->getValue()->m_offsety,
								 node->getValue()->m_dim/2,
								 node->getValue()->m_lod-1,
								 theight, twidth));
		_buildQuadTree(node->child(TerrainNode::NORTH_WEST),theight, twidth);
		_buildQuadTree(node->child(TerrainNode::SOUTH_WEST),theight, twidth);
		_buildQuadTree(node->child(TerrainNode::NORTH_EAST),theight, twidth);
		_buildQuadTree(node->child(TerrainNode::SOUTH_EAST),theight, twidth);
	}

}

void TerrainRenderer::renderQuadTree(TerrainNode* node, Node* position) {
    Transformf cam_t(position->getGlobalTransform());
    Transformf terrain_t(this->node()->getGlobalTransform());

    float patch_x=this->m_scale*(node->getValue()->m_offsetx+(node->getValue()->m_dim/2));
    float patch_y=this->m_scale*(node->getValue()->m_offsety+(node->getValue()->m_dim/2));

    float limite = node->getValue()->m_lod*60*node->getValue()->m_lod*60;
    float norme = (patch_x-cam_t.getPosition().x)*(patch_x-cam_t.getPosition().x) +
                  (patch_y-cam_t.getPosition().z)*(patch_y-cam_t.getPosition().z);

    if( limite > norme && node->getValue()->m_lod>0) {

        renderQuadTree(node->child(TerrainNode::NORTH_WEST), position);
        renderQuadTree(node->child(TerrainNode::SOUTH_WEST), position);
        renderQuadTree(node->child(TerrainNode::NORTH_EAST), position);
        renderQuadTree(node->child(TerrainNode::SOUTH_EAST), position);

	}
	else {
		node->getValue()->render();
	}
}

#ifdef WITH_TOOLS

PropertyWidget* TerrainRenderer::getWidget()
{
	if(m_widget == 0)
		m_widget = new TerrainWidget(this);

	return m_widget;
}

#endif
