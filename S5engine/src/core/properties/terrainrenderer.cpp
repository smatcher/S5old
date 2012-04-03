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

TerrainPatch::TerrainPatch(TerrainRenderer* terrain, int start_x, int start_y, int lodx, int lody, int dim, int lod, int terrain_dim):
    m_lodx(lodx),
    m_lody(lody),
    m_terrain(terrain),
    m_indices_core(QGLBuffer::IndexBuffer),
	m_offsetx(start_x),
	m_offsety(start_y),
	m_dim(dim),
    m_lod(lod),
    m_drawn(false){

	/* On saute 2^lod vertex*/
	int increment = 1<<m_lod;
	int x;
	int nx;
	int y;
	int ny;
    int index;

    GLint core_indices[PATCH_CORE_VBO_SIZE];
    GLint edge_indices_hidef[PATCH_HIRES_EDGE_VBO_SIZE];
    GLint edge_indices_lodef[PATCH_LORES_EDGE_VBO_SIZE];

    /*******************************/
    /*        Core indexes         */
    /*******************************/

    index=0;
    for(x=1; x<PATCH_DIM-1; x++) {
        for(y=1; y<PATCH_DIM-1; y++) {
            nx = x*increment + start_x;
            ny = y*increment + start_y;

            /* Triangle 1 */
            core_indices[index]	 = (nx+ny*terrain_dim);
            core_indices[index+1] = (nx+(ny+increment)*terrain_dim);
            core_indices[index+2] = ((nx+increment)+ny*terrain_dim);
            /* Triangle 2 */
            core_indices[index+3] = (nx+(ny+increment)*terrain_dim);
            core_indices[index+4] = ((nx+increment)+(ny+increment)*terrain_dim);
            core_indices[index+5] = ((nx+increment)+ny*terrain_dim);
            index += 6;
        }
    }
    m_indices_core.create();
    m_indices_core.bind();
    m_indices_core.setUsagePattern(QGLBuffer::StaticDraw);
    m_indices_core.allocate(core_indices, PATCH_CORE_VBO_SIZE*sizeof(GLint));
    m_indices_core.release();

    /*******************************/
    /*     Hires edge indexes      */
    /*******************************/

    y = 0;
    index = 0;
    for(x=0; x<PATCH_DIM; x++) {
        nx = x*increment + start_x;
        ny = y*increment + start_y;

        if(x==0) {
            /* Triangle 1 */
            edge_indices_hidef[index]	 = (nx+ny*terrain_dim);
            edge_indices_hidef[index+1] = ((nx+increment)+(ny+increment)*terrain_dim);
            edge_indices_hidef[index+2] = ((nx+increment)+ny*terrain_dim);
            index += 3;
        } else if(x==PATCH_DIM-1) {
            /* Triangle 1 */
            edge_indices_hidef[index]	 = (nx+ny*terrain_dim);
            edge_indices_hidef[index+1] = (nx+(ny+increment)*terrain_dim);
            edge_indices_hidef[index+2] = ((nx+increment)+ny*terrain_dim);
            index += 3;
        } else {
            /* Triangle 1 */
            edge_indices_hidef[index]	 = (nx+ny*terrain_dim);
            edge_indices_hidef[index+1] = (nx+(ny+increment)*terrain_dim);
            edge_indices_hidef[index+2] = ((nx+increment)+ny*terrain_dim);
            /* Triangle 2 */
            edge_indices_hidef[index+3] = (nx+(ny+increment)*terrain_dim);
            edge_indices_hidef[index+4] = ((nx+increment)+(ny+increment)*terrain_dim);
            edge_indices_hidef[index+5] = ((nx+increment)+ny*terrain_dim);
            index += 6;
        }
    }

    m_indices_edges[0][0] = QGLBuffer(QGLBuffer::IndexBuffer);
    m_indices_edges[0][0].create();
    m_indices_edges[0][0].bind();
    m_indices_edges[0][0].setUsagePattern(QGLBuffer::StaticDraw);
    m_indices_edges[0][0].allocate(edge_indices_hidef, PATCH_HIRES_EDGE_VBO_SIZE*sizeof(GLint));
    m_indices_edges[0][0].release();

    y = PATCH_DIM-1;
    index = 0;
    for(x=0; x<PATCH_DIM; x++) {
        nx = x*increment + start_x;
        ny = y*increment + start_y;

        if(x==0) {
            /* Triangle 1 */
            edge_indices_hidef[index]	 = ((nx+increment)+ny*terrain_dim);
            edge_indices_hidef[index+1] = (nx+(ny+increment)*terrain_dim);((nx+increment)+(ny+increment)*terrain_dim);
            edge_indices_hidef[index+2] = ((nx+increment)+(ny+increment)*terrain_dim);
            index += 3;
        } else if(x==PATCH_DIM-1) {
            /* Triangle 1 */
            edge_indices_hidef[index]	 = (nx+ny*terrain_dim);
            edge_indices_hidef[index+1] = (nx+(ny+increment)*terrain_dim);
            edge_indices_hidef[index+2] = ((nx+increment)+(ny+increment)*terrain_dim);
            index += 3;
        } else {
            /* Triangle 1 */
            edge_indices_hidef[index]	 = (nx+ny*terrain_dim);
            edge_indices_hidef[index+1] = (nx+(ny+increment)*terrain_dim);
            edge_indices_hidef[index+2] = ((nx+increment)+ny*terrain_dim);
            /* Triangle 2 */
            edge_indices_hidef[index+3] = (nx+(ny+increment)*terrain_dim);
            edge_indices_hidef[index+4] = ((nx+increment)+(ny+increment)*terrain_dim);
            edge_indices_hidef[index+5] = ((nx+increment)+ny*terrain_dim);
            index += 6;
        }
    }

    m_indices_edges[1][0] = QGLBuffer(QGLBuffer::IndexBuffer);
    m_indices_edges[1][0].create();
    m_indices_edges[1][0].bind();
    m_indices_edges[1][0].setUsagePattern(QGLBuffer::StaticDraw);
    m_indices_edges[1][0].allocate(edge_indices_hidef, PATCH_HIRES_EDGE_VBO_SIZE*sizeof(GLint));
    m_indices_edges[1][0].release();

    x = 0;
    index = 0; 
    for(y=0; y<PATCH_DIM; y++) {
        nx = x*increment + start_x;
        ny = y*increment + start_y;

        if(y==0) {
            /* Triangle 1 */
            edge_indices_hidef[index]	 = (nx+ny*terrain_dim);
            edge_indices_hidef[index+1] = (nx+(ny+increment)*terrain_dim);
            edge_indices_hidef[index+2] = ((nx+increment)+(ny+increment)*terrain_dim);
            index += 3;
        } else if(y==PATCH_DIM-1) {
            /* Triangle 1 */
            edge_indices_hidef[index]	 = (nx+ny*terrain_dim);
            edge_indices_hidef[index+1] = (nx+(ny+increment)*terrain_dim);
            edge_indices_hidef[index+2] = ((nx+increment)+ny*terrain_dim);
            index += 3;
        } else {
            /* Triangle 1 */
            edge_indices_hidef[index]	 = (nx+ny*terrain_dim);
            edge_indices_hidef[index+1] = (nx+(ny+increment)*terrain_dim);
            edge_indices_hidef[index+2] = ((nx+increment)+ny*terrain_dim);
            /* Triangle 2 */
            edge_indices_hidef[index+3] = (nx+(ny+increment)*terrain_dim);
            edge_indices_hidef[index+4] = ((nx+increment)+(ny+increment)*terrain_dim);
            edge_indices_hidef[index+5] = ((nx+increment)+ny*terrain_dim);
            index += 6;
        }
    }

    m_indices_edges[2][0] = QGLBuffer(QGLBuffer::IndexBuffer);
    m_indices_edges[2][0].create();
    m_indices_edges[2][0].bind();
    m_indices_edges[2][0].setUsagePattern(QGLBuffer::StaticDraw);
    m_indices_edges[2][0].allocate(edge_indices_hidef, PATCH_HIRES_EDGE_VBO_SIZE*sizeof(GLint));
    m_indices_edges[2][0].release();

    x = PATCH_DIM-1;
    index = 0;
    for(y=0; y<PATCH_DIM; y++) {
        nx = x*increment + start_x;
        ny = y*increment + start_y;

        if(y==0) {
            /* Triangle 1 */
            edge_indices_hidef[index]	 = (nx+(ny+increment)*terrain_dim);
            edge_indices_hidef[index+1] = ((nx+increment)+(ny+increment)*terrain_dim);
            edge_indices_hidef[index+2] = ((nx+increment)+ny*terrain_dim);
            index += 3;
        } else if(y==PATCH_DIM-1) {
            /* Triangle 1 */
            edge_indices_hidef[index]	 = (nx+ny*terrain_dim);
            edge_indices_hidef[index+1] = ((nx+increment)+(ny+increment)*terrain_dim);
            edge_indices_hidef[index+2] = ((nx+increment)+ny*terrain_dim);
            index += 3;
        } else {
            /* Triangle 1 */
            edge_indices_hidef[index]	 = (nx+ny*terrain_dim);
            edge_indices_hidef[index+1] = (nx+(ny+increment)*terrain_dim);
            edge_indices_hidef[index+2] = ((nx+increment)+ny*terrain_dim);
            /* Triangle 2 */
            edge_indices_hidef[index+3] = (nx+(ny+increment)*terrain_dim);
            edge_indices_hidef[index+4] = ((nx+increment)+(ny+increment)*terrain_dim);
            edge_indices_hidef[index+5] = ((nx+increment)+ny*terrain_dim);
            index += 6;
        }
    }

    m_indices_edges[3][0] = QGLBuffer(QGLBuffer::IndexBuffer);
    m_indices_edges[3][0].create();
    m_indices_edges[3][0].bind();
    m_indices_edges[3][0].setUsagePattern(QGLBuffer::StaticDraw);
    m_indices_edges[3][0].allocate(edge_indices_hidef, PATCH_HIRES_EDGE_VBO_SIZE*sizeof(GLint));
    m_indices_edges[3][0].release();

    /*******************************/
    /*     LoRes edge indexes      */
    /*******************************/

    y = 0;
    index = 0;
    for(x=0; x<PATCH_DIM; x+=2) {
        nx = x*increment + start_x;
        ny = y*increment + start_y;
        if(x>0) {
            /* Triangle 1 */
            edge_indices_lodef[index]	 = (nx+ny*terrain_dim);
            edge_indices_lodef[index+1] = (nx+(ny+increment)*terrain_dim);
            edge_indices_lodef[index+2] = ((nx+increment)+(ny+increment)*terrain_dim);
            index += 3;
        }
        /* Triangle 2 */
        edge_indices_lodef[index] = (nx+ny*terrain_dim);
        edge_indices_lodef[index+1] = ((nx+increment)+(ny+increment)*terrain_dim);
        edge_indices_lodef[index+2] = ((nx+2*increment)+ny*terrain_dim);
        index += 3;

        if(x<PATCH_DIM-1) {
            /* Triangle 3 */
            edge_indices_lodef[index] = ((nx+increment)+(ny+increment)*terrain_dim);
            edge_indices_lodef[index+1] = ((nx+2*increment)+(ny+increment)*terrain_dim);
            edge_indices_lodef[index+2] = ((nx+2*increment)+ny*terrain_dim);
            index += 3;
        }
    }

    m_indices_edges[0][1] = QGLBuffer(QGLBuffer::IndexBuffer);
    m_indices_edges[0][1].create();
    m_indices_edges[0][1].bind();
    m_indices_edges[0][1].setUsagePattern(QGLBuffer::StaticDraw);
    m_indices_edges[0][1].allocate(edge_indices_lodef, PATCH_LORES_EDGE_VBO_SIZE*sizeof(GLint));
    m_indices_edges[0][1].release();

    y = PATCH_DIM-1;
    index = 0;
    for(x=0; x<PATCH_DIM; x+=2) {
        nx = x*increment + start_x;
        ny = y*increment + start_y;
        if(x>0) {
            /* Triangle 1 */
            edge_indices_lodef[index]	 = (nx+ny*terrain_dim);
            edge_indices_lodef[index+1] = (nx+(ny+increment)*terrain_dim);
            edge_indices_lodef[index+2] = ((nx+increment)+ny*terrain_dim);
            index += 3;
        }
        /* Triangle 2 */
        edge_indices_lodef[index] = (nx+(ny+increment)*terrain_dim);
        edge_indices_lodef[index+1] = ((nx+2*increment)+(ny+increment)*terrain_dim);
        edge_indices_lodef[index+2] = ((nx+increment)+ny*terrain_dim);
        index += 3;

        if(x<PATCH_DIM-1) {
            /* Triangle 3 */
            edge_indices_lodef[index] = ((nx+increment)+ny*terrain_dim);
            edge_indices_lodef[index+1] = ((nx+2*increment)+(ny+increment)*terrain_dim);
            edge_indices_lodef[index+2] = ((nx+2*increment)+ny*terrain_dim);
            index += 3;
        }
    }

    m_indices_edges[1][1] = QGLBuffer(QGLBuffer::IndexBuffer);
    m_indices_edges[1][1].create();
    m_indices_edges[1][1].bind();
    m_indices_edges[1][1].setUsagePattern(QGLBuffer::StaticDraw);
    m_indices_edges[1][1].allocate(edge_indices_lodef, PATCH_LORES_EDGE_VBO_SIZE*sizeof(GLint));
    m_indices_edges[1][1].release();

    x = 0;
    index = 0;
    for(y=0; y<PATCH_DIM; y+=2) {
        nx = x*increment + start_x;
        ny = y*increment + start_y;
        if(y>0) {
            /* Triangle 1 */
            edge_indices_lodef[index]	 = (nx+ny*terrain_dim);
            edge_indices_lodef[index+1] = ((nx+increment)+(ny+increment)*terrain_dim);
            edge_indices_lodef[index+2] = ((nx+increment)+ny*terrain_dim);
            index += 3;
        }
        /* Triangle 2 */
        edge_indices_lodef[index] = (nx+ny*terrain_dim);
        edge_indices_lodef[index+1] = (nx+(ny+2*increment)*terrain_dim);
        edge_indices_lodef[index+2] = ((nx+increment)+(ny+increment)*terrain_dim);
        index += 3;

        if(y<PATCH_DIM-1) {
            /* Triangle 3 */
            edge_indices_lodef[index] = ((nx+increment)+(ny+increment)*terrain_dim);
            edge_indices_lodef[index+1] = (nx+(ny+2*increment)*terrain_dim);
            edge_indices_lodef[index+2] = ((nx+increment)+(ny+2*increment)*terrain_dim);
            index += 3;
        }
    }

    m_indices_edges[2][1] = QGLBuffer(QGLBuffer::IndexBuffer);
    m_indices_edges[2][1].create();
    m_indices_edges[2][1].bind();
    m_indices_edges[2][1].setUsagePattern(QGLBuffer::StaticDraw);
    m_indices_edges[2][1].allocate(edge_indices_lodef, PATCH_LORES_EDGE_VBO_SIZE*sizeof(GLint));
    m_indices_edges[2][1].release();

    x = PATCH_DIM-1;
    index = 0;
    for(y=0; y<PATCH_DIM; y+=2) {
        nx = x*increment + start_x;
        ny = y*increment + start_y;
        if(y>0) {
            /* Triangle 1 */
            edge_indices_lodef[index]	 = (nx+ny*terrain_dim);
            edge_indices_lodef[index+1] = (nx+(ny+increment)*terrain_dim);
            edge_indices_lodef[index+2] = ((nx+increment)+ny*terrain_dim);
            index += 3;
        }
        /* Triangle 2 */
        edge_indices_lodef[index] = ((nx+increment)+ny*terrain_dim);
        edge_indices_lodef[index+1] = (nx+(ny+increment)*terrain_dim);
        edge_indices_lodef[index+2] = ((nx+increment)+(ny+2*increment)*terrain_dim);
        index += 3;

        if(y<PATCH_DIM-1) {
            /* Triangle 3 */
            edge_indices_lodef[index] = (nx+(ny+increment)*terrain_dim);
            edge_indices_lodef[index+1] = (nx+(ny+2*increment)*terrain_dim);
            edge_indices_lodef[index+2] = ((nx+increment)+(ny+2*increment)*terrain_dim);
            index += 3;
        }
    }
    m_indices_edges[3][1] = QGLBuffer(QGLBuffer::IndexBuffer);
    m_indices_edges[3][1].create();
    m_indices_edges[3][1].bind();
    m_indices_edges[3][1].setUsagePattern(QGLBuffer::StaticDraw);
    m_indices_edges[3][1].allocate(edge_indices_lodef, PATCH_LORES_EDGE_VBO_SIZE*sizeof(GLint));
    m_indices_edges[3][1].release();
}

struct neighbour_data{
    int nx,ny;
    int edge_index;
};

void TerrainPatch::drawAdaptedEdge(int nx, int ny, int edge) {
    int lod = 0;
    if(nx>=0 && nx<(1<<(m_terrain->getMaxLod()-(m_lod+1))) && ny>=0 && ny<(1<<(m_terrain->getMaxLod()-(m_lod+1)))){
        if(m_terrain->getLodDrawn(m_lod+1,nx,ny)) {
            lod = 1;
        }
    }

    m_indices_edges[edge][lod].bind();
    glIndexPointer(GL_INT, 0, NULL);
    if(lod)
        glDrawElements(GL_TRIANGLES, PATCH_LORES_EDGE_VBO_SIZE, GL_UNSIGNED_INT, NULL);
    else
        glDrawElements(GL_TRIANGLES, PATCH_HIRES_EDGE_VBO_SIZE, GL_UNSIGNED_INT, NULL);
    m_indices_edges[edge][lod].release();
}

void TerrainPatch::drawNormalEdge(int edge) {
    m_indices_edges[edge][0].bind();
    glIndexPointer(GL_INT, 0, NULL);
    glDrawElements(GL_TRIANGLES, PATCH_HIRES_EDGE_VBO_SIZE, GL_UNSIGNED_INT, NULL);
    m_indices_edges[edge][0].release();
}

void TerrainPatch::render() {
	glEnableClientState(GL_INDEX_ARRAY);
    int x,y,nx,ny,px,py,lod;

    m_indices_core.bind();
	glIndexPointer(GL_INT, 0, NULL);
    glDrawElements(GL_TRIANGLES, PATCH_CORE_VBO_SIZE, GL_UNSIGNED_INT, NULL);
    m_indices_core.release();

    /* Position au sein du quad de niveau supérieur (0 ou 1 pour x et y) */
    x = m_lodx%2;
    y = m_lody%2;

    nx = m_lodx/2;
    ny = m_lody/2;

    if(x==0) {
        drawAdaptedEdge(nx-1,ny,2);
        drawNormalEdge(3);
    } else {
        drawAdaptedEdge(nx+1,ny,3);
        drawNormalEdge(2);
    }

    if(y==0) {
        drawAdaptedEdge(nx,ny-1,0);
        drawNormalEdge(1);
    } else {
        drawAdaptedEdge(nx,ny+1,1);
        drawNormalEdge(0);
    }

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

	/* Pas terrible, vue que la Texture n'est pas FORCEMENT une StbImage, Ã  amÃ©liorer donc... */
	stbi_uc* image = ((StbImage*)*hm)->getData();

	GLfloat* vertices;
	GLfloat* normals;
	GLfloat* texcoords;
	GLfloat* stexcoords;

	int index;
	int comp = ((StbImage*)*hm)->getComp();

    logInfo( "Computing vertices" );
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

    logInfo( "Computing normals" );
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

			normal *= -1.0f; /* Ouais j'ai calculÃ© les normales Ã  l'envers, a corriger quand j'aurais pas la fleme */
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

    logInfo( "Computing texcoord" );
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

    logInfo( "Computing stexcoord" );
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

    logInfo( "Computing quadtree" );
    switch(m_height-1) {
        case 256:
            m_maxlod = 4;
            break;
        case 512:
            m_maxlod = 5;
            break;
        case 1024:
            m_maxlod = 6;
            break;
        case 2048:
            m_maxlod = 7;
            break;
        case 4096:
            m_maxlod = 8;
            break;
        default:
        logError("Heightmap dimensions not supported.");
        exit(0);
    }

    buildQuadTree();
    m_lod_drawn = (bool**)malloc((m_maxlod+1)*sizeof(bool*));
    for(int i=0; i<m_maxlod+1; i++){
        m_lod_drawn[i] = (bool*)malloc((1<<i)*(1<<i)*sizeof(bool));
        memset(m_lod_drawn[i], false, (1<<i)*(1<<i)*sizeof(bool));
    }

    logInfo( "Terrain created!" );

    delete[] vertices;
    delete[] normals;
    delete[] texcoords;
    delete[] stexcoords;


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
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		__ClientActiveTexture(GL_TEXTURE1);
		m_stexcoords.bind();
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
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	m_vertices.bind();
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if(m_wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	//m_quadtree->getValue()->render();
    evalLOD(m_quadtree, cam_node);
    //logInfo("<<<map");
    //printLodMap();
    renderQuadTree(m_quadtree);

	if(m_wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	m_vertices.release();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);

	/* DÃ©sactivation du material */
	m_material->unset(0);
}

int TerrainRenderer::getMaxLod() {
    return m_maxlod;
}

bool TerrainRenderer::getLodDrawn(int lod, int x, int y) {
    int dim = 1<<(-lod+m_maxlod);
    return m_lod_drawn[(-lod+m_maxlod)][x+y*dim];
}

void TerrainRenderer::setLodDrawn(int lod, int x, int y, bool value) {
    int dim = 1<<(-lod+m_maxlod);
    m_lod_drawn[(-lod+m_maxlod)][x+y*dim] = value;
}

void TerrainRenderer::buildQuadTree() {
    m_quadtree = new TerrainNode(new TerrainPatch(this,0,0,0,0,m_height,m_maxlod,m_height), 0, TerrainNode::ROOT);
    _buildQuadTree(m_quadtree, m_height, m_width, 0, 0);
}

void TerrainRenderer::_buildQuadTree(TerrainNode* node, int theight, int twidth, int lodx, int lody) {
    if(node->getValue()->m_lod >= 0) {
        node->addChild(TerrainNode::NORTH_WEST, new TerrainPatch(this,node->getValue()->m_offsetx,
                                                                 node->getValue()->m_offsety,
                                                                 2*lodx, 2*lody,
                                                                 node->getValue()->m_dim/2,
                                                                 node->getValue()->m_lod-1,
                                                                 theight));
        node->addChild(TerrainNode::SOUTH_WEST, new TerrainPatch(this,node->getValue()->m_offsetx,
                                                                 node->getValue()->m_offsety + node->getValue()->m_dim/2,
                                                                 2*lodx,2*lody+1,
                                                                 node->getValue()->m_dim/2,
                                                                 node->getValue()->m_lod-1,
                                                                 theight));
        node->addChild(TerrainNode::NORTH_EAST, new TerrainPatch(this,node->getValue()->m_offsetx + node->getValue()->m_dim/2,
                                                                 node->getValue()->m_offsety,
                                                                 2*lodx+1, 2*lody,
                                                                 node->getValue()->m_dim/2,
                                                                 node->getValue()->m_lod-1,
                                                                 theight));
        node->addChild(TerrainNode::SOUTH_EAST, new TerrainPatch(this,node->getValue()->m_offsetx  + node->getValue()->m_dim/2,
                                                                 node->getValue()->m_offsety + node->getValue()->m_dim/2,
                                                                 2*lodx+1, 2*lody+1,
                                                                 node->getValue()->m_dim/2,
                                                                 node->getValue()->m_lod-1,
                                                                 theight));
        _buildQuadTree(node->child(TerrainNode::NORTH_WEST),theight, twidth, 2*lodx, 2*lody);
        _buildQuadTree(node->child(TerrainNode::SOUTH_WEST),theight, twidth, 2*lodx, 2*lody+1);
        _buildQuadTree(node->child(TerrainNode::NORTH_EAST),theight, twidth, 2*lodx+1, 2*lody);
        _buildQuadTree(node->child(TerrainNode::SOUTH_EAST),theight, twidth, 2*lodx+1, 2*lody+1);
	}

}

void TerrainRenderer::evalLOD(TerrainNode* node, Node* position) {
    Transformf cam_t(position->getGlobalTransform());
    Transformf terrain_t(this->node()->getGlobalTransform());

    /*XXX Calcul de la distance en 2D, pabo*/
    float patch_x=this->m_scale*(node->getValue()->m_offsetx+((float)node->getValue()->m_dim/2.0f));
    float patch_y=this->m_scale*(node->getValue()->m_offsety+((float)node->getValue()->m_dim/2.0f));

    float norme = sqrt((patch_x-cam_t.getPosition().x)*(patch_x-cam_t.getPosition().x) +
                  (patch_y-cam_t.getPosition().z)*(patch_y-cam_t.getPosition().z));

    /* Calcul de la limite avant le LOD suivant. Cette limite est calculé suivant une suite arithmétique Un+1 = Un*2. U0 peut être changé pour régler la qualité du terrain */
    float limite = 50*(1<<node->getValue()->m_lod);

    if( limite > norme && node->getValue()->m_lod>0) {
        node->getValue()->m_drawn = false;
        setLodDrawn(node->getValue()->m_lod,node->getValue()->m_lodx,node->getValue()->m_lody, false);

        evalLOD(node->child(TerrainNode::NORTH_WEST), position);
        evalLOD(node->child(TerrainNode::SOUTH_WEST), position);
        evalLOD(node->child(TerrainNode::NORTH_EAST), position);
        evalLOD(node->child(TerrainNode::SOUTH_EAST), position);
    }
    else {
        node->getValue()->m_drawn = true;
        setLodDrawn(node->getValue()->m_lod,node->getValue()->m_lodx,node->getValue()->m_lody, true);
    }
}


void TerrainRenderer::renderQuadTree(TerrainNode* node) {

    if(!node->getValue()->m_drawn){
        renderQuadTree(node->child(TerrainNode::NORTH_WEST));
        renderQuadTree(node->child(TerrainNode::SOUTH_WEST));
        renderQuadTree(node->child(TerrainNode::NORTH_EAST));
        renderQuadTree(node->child(TerrainNode::SOUTH_EAST));
	}
	else {
		node->getValue()->render();
	}
}

#ifdef WITH_TOOLS

PropertyWidget* TerrainRenderer::getWidget()
{
	if(m_widget == NULL)
		m_widget = new TerrainWidget(this);

	return m_widget;
}

#endif
