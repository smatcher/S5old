#ifndef TERRAIN_H
#define TERRAIN_H

#include <core/properties/irenderable.h>
#include <core/graphics/texture.h>

class TerrainRenderer : public IRenderable
{
public:
	TerrainRenderer(Texture& hm);
	void render(double elapsed_time, GLWidget* context);
	bool isTransparent() {return false;}

private:
	float* heightmap;
};

#endif // TERRAIN_H