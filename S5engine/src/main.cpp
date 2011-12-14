#include <QApplication>

#include "core/framework/engine.h"
#include "core/scenegraph/scenegraph.h"
#include "core/scenegraph/node.h"
#include "core/maths/trigo.h"
#include "core/properties/dummyupdatable.h"
#include "core/properties/dummycontrolable.h"
#include "core/properties/camera.h"
#include "core/properties/light.h"
#include "core/properties/terrainrenderer.h"
#include "core/properties/soundemitter.h"
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

	Engine engine(argc, argv);

	SceneGraph* sg = engine.getScenegraph_TEMPORARY();

	Node* nRot = new Node("Rotating node");
	Node* nCam = new Node("Camera");
	Node* nBall = new Node("Ball");
	Node* nLight = new Node("Light");


	nRot->addProperty(new IProperty());
	nRot->addProperty(new DummyUpdatable());
	nCam->addProperty(new Camera(70,1,200));
	nLight->addProperty(new Light());


	Mesh sphere = MESH_MANAGER.get("Sphere_16_32");
	Material ball = MATERIAL_MANAGER.get("ball");

	nBall->addProperty(new MeshRenderer(sphere,ball));

	sg->link(nRot);
	sg->link(nBall);
	nRot->link(nCam);
	nRot->link(nLight);

	nRot->moveTo(Vector3f(0,0,0));
	nCam->moveTo(Vector3f(0,1,2));
	nCam->rotate(Vector3f(1,0,0),330);

	nLight->moveTo(Vector3f(0,0,1));

	// Beurk ! Mais je peux le faire alors je me prive pas ^^
	RENDER_MANAGER.setCurrentCamera(static_cast<Camera*>(nCam->properties().child("Camera")));
	RenderManager::Background background;
	background.type = RenderManager::Background::COLOR;
	background.color = Vector3f(0,0,0);
	RENDER_MANAGER.setBackground(background);

	int ret = engine.start();

	return ret;
}
