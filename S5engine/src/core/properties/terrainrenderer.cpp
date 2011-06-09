#include <core/properties/terrainrenderer.h>

TerrainRenderer::TerrainRenderer(Texture& hm): IRenderable("TerrainRenderer") {
	debug( "RESOURCE PARSING" ,"Creating terrain (" << hm.getComp() << ")"  );
}

void TerrainRenderer::render(double elapsed_time, GLWidget* context) {

}
