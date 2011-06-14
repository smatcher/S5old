#include <QApplication>

#include "core/framework/engine.h"
#include "core/scenegraph/scenegraph.h"
#include "core/scenegraph/node.h"
#include "core/maths/trigo.h"
#include "core/properties/dummyupdatable.h"
#include "core/properties/dummycontrolable.h"
#include "core/properties/physicobject.h"
#include "core/properties/qtlogo.h"
#include "core/properties/camera.h"
#include "core/properties/light.h"
#include "core/properties/soundemitter.h"
#include "core/properties/terrainrenderer.h"
#include "core/managers/rendermanager.h"
#include "core/properties/grid.h"
#ifdef Q_WS_X11
	#include <X11/Xlib.h>
#endif

#include "core/resources/managers.h"
#include "core/properties/meshrenderer.h"

#include "core/inputs/inputmanager.h"

#include <AL/al.h>
#include <AL/alc.h>
//#include <AL/alu.h>
#include <AL/alut.h>



int main(int argc, char *argv[])
{
	#ifdef Q_WS_X11
		XInitThreads();
	#endif

	Engine engine(argc, argv,"demo_game/media/");

	SceneGraph* sg = engine.getScenegraph_TEMPORARY();

	Node* nTerrain = new Node("Terrain");
	Node* nRot = new Node("Rotating node");
	Node* nLight = new Node("Light");

	Texture heightmap;

	if(argc>1)
		heightmap = TEXTURE_MANAGER.get(argv[1]);
	else
		heightmap = TEXTURE_MANAGER.get("heightmap2.jpg");

	Material terrain = MATERIAL_MANAGER.get("terrain_test");

	nTerrain->addProperty(new TerrainRenderer(heightmap, terrain, 100.0f, 1.f,20.f));
	nTerrain->move(Vector3f(-256.f, -100.f, -256.f));

	nLight->addProperty(new Light());
	nLight->moveTo(Vector3f(4.0f, 4.0f, 0.0f));

	nRot->addProperty(new DummyUpdatable());
	nRot->setScale(Vector3f(20.0f, 20.0f, 20.0f));
	sg->link(nTerrain);
	nRot->link(nLight);
	sg->link(nRot);

	RenderManager::Background background;
	background.type = RenderManager::SKYBOX;
	background.color = Vector3f(1,1,0);
	background.textures[0] = TEXTURE_MANAGER.get("stormy_front.tga");
	background.textures[1] = TEXTURE_MANAGER.get("stormy_left.tga");
	background.textures[2] = TEXTURE_MANAGER.get("stormy_back.tga");
	background.textures[3] = TEXTURE_MANAGER.get("stormy_right.tga");
	background.textures[4] = TEXTURE_MANAGER.get("stormy_top.tga");
	background.textures[5] = TEXTURE_MANAGER.get("stormy_bottom.tga");
	/*
	background.textures[0] = TEXTURE_MANAGER.get("interstellar_lf.tga");
	background.textures[1] = TEXTURE_MANAGER.get("interstellar_ft.tga");
	background.textures[2] = TEXTURE_MANAGER.get("interstellar_rt.tga");
	background.textures[3] = TEXTURE_MANAGER.get("interstellar_bk.tga");
	background.textures[4] = TEXTURE_MANAGER.get("interstellar_up.tga");
	background.textures[5] = TEXTURE_MANAGER.get("interstellar_dn.tga");
	*/
	RENDER_MANAGER.setBackground(background);

	int ret = engine.start();

	return ret;
}
