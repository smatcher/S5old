#include <QApplication>

#include "core/framework/engine.h"
#include "core/scenegraph/scenegraph.h"
#include "core/scenegraph/node.h"
#include "core/maths/trigo.h"
#include "core/properties/dummyupdatable.h"
#include "core/properties/dummycontrolable.h"
#include "core/properties/physicobject.h"
#include "core/properties/camera.h"
#include "core/properties/light.h"
#include "core/properties/soundemitter.h"
#include "core/properties/terrainrenderer.h"
#include "core/managers/rendermanager.h"
#include "core/properties/grid.h"
#ifdef Q_WS_X11
	#include <X11/Xlib.h>
#endif

#include "planecontroller.h"

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

	QList<InputManager::Control> controls;
	controls.push_back(InputManager::Control(false,"avance"));
	controls.push_back(InputManager::Control(false,"recule"));
	controls.push_back(InputManager::Control(false,"gauche"));
	controls.push_back(InputManager::Control(false,"droite"));
	INPUT_MANAGER.addControls(controls);

	INPUT_MANAGER.addBinding("KB_Z","avance");
	INPUT_MANAGER.addBinding("KB_Up","avance");
	INPUT_MANAGER.addBinding("KB_S","recule");
	INPUT_MANAGER.addBinding("KB_Down","recule");
	INPUT_MANAGER.addBinding("KB_Q","gauche");
	INPUT_MANAGER.addBinding("KB_Left","gauche");
	INPUT_MANAGER.addBinding("KB_D","droite");
	INPUT_MANAGER.addBinding("KB_Right","droite");


	Node* nTerrain = new Node("Terrain");
	Node* nBase = new Node("Base");
	Node* nRot = new Node("Rotating node");
	Node* nLight = new Node("Light");
	Node* nPlane = new Node("Plane");
	Node* nCentre = new Node("Centre");
	Node* nSphere = new Node("Sphere");
	Node* nCamFollow = new Node("Camera Follow");

	Texture heightmap;

	if(argc>1) {
		heightmap = TEXTURE_MANAGER.get(argv[1]);
	 } else {
		heightmap = TEXTURE_MANAGER.get("heightmap.tga");
	}


	Mesh sphere = MESH_MANAGER.get("Sphere_16_32");
	Mesh palm = MESH_MANAGER.get("palm");
	Mesh mesh = MESH_MANAGER.get("duckplane");
	Material mat = MATERIAL_MANAGER.get("palm");
	Material duckmat = MATERIAL_MANAGER.get("duckplane");
	Material terrain = MATERIAL_MANAGER.get("terrain_test");

	PhysicObject::Properties prop;
	prop.is_kinematic = false;
	prop.mass = 100.0;
	prop.restitution = 0.1;
	//prop.linDamping = 0.6;
	//prop.angDamping = 0.3;
	prop.shape = PhysicObject::MESH;
	prop.mesh_name = "duckplane";
	sg->link(nCentre);
	nCentre->link(nPlane);
	nCentre->move(Vector3f(512.0,50.0,512.0));
	nPlane->addProperty(new MeshRenderer(mesh,duckmat));
	nPlane->rotate(Vector3f(0.0,0.0,1.0), 20.0);
	nPlane->move(Vector3f(256.0,20.0,0.0));
	//nPlane->setScale(Vector3f(5.0,5.0,5.0));
	nCentre->addProperty(new DummyUpdatable(0.1));

	Camera* cam = new Camera(90,1,1024);
	nCamFollow->addProperty(cam);
	nCamFollow->move(Vector3f(0,2,5));
	nCamFollow->rotate(Vector3f(-1,0,0),15);
	nPlane->link(nCamFollow);

	Light* light = new Light(true);
	light->setAttenuation(0,1.f/512.f,0);
	//light->setAttenuation(0.1,0,0);
	light->setType(Light::SUN);
	nLight->addProperty(light);
	nSphere->addProperty(new MeshRenderer(palm,mat));
	nSphere->rotate(Vector3f(1,0,0),-90);
	//nSphere->addProperty(new MeshRenderer(sphere,mat));
/*
	nLight->move(Vector3f(512.0, 75.0, 512.0));
	nLight->rotate(Vector3f(0.0, 0.0, 1.0),35);
*/
	nLight->lookAt(Vector3f(4.3,13,10));
	nLight->rotate(nLight->apply(Vector3f(0,1,0)),90);
	nLight->moveTo(20*Vector3f(4.3,13,10) + Vector3f(512,50,512));
	//nLight->lookAt(Vector3f(512,50,512));
	//nLight->rotate(Vector3f(0,1,0),-90);
	cam = new Camera(90,1,1024);
	nLight->addProperty(cam);

	nBase->rotate(Vector3f(1,0,0),90);
	nBase->rotate(Vector3f(0,1,0),-45);

	nTerrain->addProperty(new TerrainRenderer(heightmap, terrain, 70.0f, 2.f,20.f));

	sg->link(nTerrain);
	sg->link(nLight);
	nBase->link(nRot);
	sg->link(nBase);
	nCentre->link(nSphere);
	nSphere->setScale(Vector3f(7,7,7));

	RenderManager::Background background;
	background.type = RenderManager::Background::COLOR;
	background.color = Vector3f(0,0,0);
	background.sunDirection = Vector3f(4.3,13,10);
	background.textures[0] = TEXTURE_MANAGER.get("interstellar_lf.tga");
	background.textures[1] = TEXTURE_MANAGER.get("interstellar_ft.tga");
	background.textures[2] = TEXTURE_MANAGER.get("interstellar_rt.tga");
	background.textures[3] = TEXTURE_MANAGER.get("interstellar_bk.tga");
	background.textures[4] = TEXTURE_MANAGER.get("interstellar_up.tga");
	background.textures[5] = TEXTURE_MANAGER.get("interstellar_dn.tga");
	background.type = RenderManager::Background::SKYBOX;

	RENDER_MANAGER.setBackground(background);
	RENDER_MANAGER.setCurrentCamera(cam);
	RENDER_MANAGER.setAmbient(Vector3f(0.1,0.2,0.15));

	int ret = engine.start();

	return ret;
}
