#include <QApplication>

#include "core/framework/engine.h"
#include "core/scenegraph/scenegraph.h"
#include "core/scenegraph/node.h"
#include "core/maths/trigo.h"
#include "core/properties/dummyupdatable.h"
#include "core/properties/camera.h"
#include "core/properties/light.h"
#include "core/managers/rendermanager.h"
#ifdef Q_WS_X11
	#include <X11/Xlib.h>
#endif

#include "core/resources/managers.h"
#include "core/properties/meshrenderer.h"

#include "dummyjumper.h"
#include "trailrenderer.h"

int main(int argc, char *argv[])
{
	#ifdef Q_WS_X11
		XInitThreads();
	#endif

	Engine engine(argc, argv,"demo_game/media/");

	SceneGraph* sg = engine.getScenegraph_TEMPORARY();

	Node* nCamFollow = new Node("Camera Follow");
	Node* nLight = new Node("Light");
	Node* nNyan = new Node("Nyan");
	Node* nRot = new Node("Rot");
	Node* nJumpy = new Node("Jumpy");
	Node* nTrailer = new Node("Trailer");

	Mesh nyan_mesh = MESH_MANAGER.get("nyan");
	Material nyan_material = MATERIAL_MANAGER.get("nyan");
	Material rainbow_material = MATERIAL_MANAGER.get("rainbow");

	nCamFollow->addProperty(new Camera(70,1,200));
	nLight->addProperty(new Light());
	nNyan->addProperty(new MeshRenderer(nyan_mesh,nyan_material));
	nTrailer->addProperty(new TrailRenderer(rainbow_material));
	nRot->addProperty(new DummyUpdatable(-0.4));
	nJumpy->addProperty(new DummyJumper(1, 0.7));

	sg->link(nRot);
	nRot->link(nCamFollow);
	nRot->link(nLight);
	nRot->link(nJumpy);
	nJumpy->link(nNyan);
	nNyan->link(nTrailer);

	nJumpy->moveTo(Vector3f(-5,0,0));

	nTrailer->moveTo(Vector3f(-0.5,0.25,0));
	nTrailer->setScale(Vector3f(1.0,0.8,1.0));

	nCamFollow->moveTo(Vector3f(-2,0,0));
	nCamFollow->rotate(Vector3f(0,0,1),-5);
	nCamFollow->rotate(Vector3f(0,1,0),90);

	nLight->moveTo(Vector3f(4,0,-1));

	// Beurk ! Mais je peux le faire alors je me prive pas ^^
	RENDER_MANAGER.setCurrentCamera(static_cast<Camera*>(nCamFollow->properties().child("Camera")));
	RenderManager::Background background;
	background.type = RenderManager::COLOR;
	background.color = Vector3f(0.031,0.259,0.451);
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
