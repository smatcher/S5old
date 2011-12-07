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
#include "core/managers/rendermanager.h"
#include "core/properties/grid.h"
#ifdef Q_WS_X11
	#include <X11/Xlib.h>
#endif

#include "core/resources/managers.h"
#include "core/properties/meshrenderer.h"

#include "core/managers/commandmanager.h"

#include "core/inputs/inputmanager.h"

#include <AL/al.h>
#include <AL/alc.h>
//#include <AL/alu.h>
#include <AL/alut.h>

SceneGraph* g_sg = NULL;

bool spawn_duck(QStringList args)
{
	Material mat = MATERIAL_MANAGER.get("duck");
	Mesh mesh = MESH_MANAGER.get("duck");
	Node* node = new Node("a duck");
	g_sg->link(node);
	node->moveTo(Vector3f(0,6,0));

	node->addProperty(new MeshRenderer(mesh, mat));
	PhysicObject::Properties prop;
	prop.is_kinematic = false;
	prop.mass = 100.0;
	prop.restitution = 0.1;
	prop.shape = PhysicObject::MESH;
	prop.mesh_name = "duck";
	node->addProperty(new PhysicObject(prop));

	return true;
}

int main(int argc, char *argv[])
{
	bool shadow_mapping = true;

	#ifdef Q_WS_X11
		XInitThreads();
	#endif

	Engine engine(argc, argv,"demo_game/media/");

	SceneGraph* sg = engine.getScenegraph_TEMPORARY();
	g_sg = sg;

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

	Node* nRot = new Node("Rotating node");
	Node* nQt = new Node("Qt Logo");
	Node* nCam = new Node("Camera");
	Node* nCamFollow = new Node("Camera Follow");
	Node* nHead = new Node("Head");
	Node* nStar = new Node("Star");
	Node* nLight = new Node("Light");
	Node* nLight2 = new Node("Light2");
	Node* nUni = new Node("Unicorn");
	Node* nGarg = new Node("Gargoyle");
	Node* nDuck = new Node("Duck");
	//Node* nDuckGrid = new Node("Grid");
	Node* nSand = new Node("Sand");

	QVector<Node*> balls;
	QVector<Node*> walls;

	for(int i=0 ; i<4 ; i++) {
		walls.push_back(new Node("Wall" + QString().setNum(i+1)));
	}
	for(int i=0 ; i<5 ; i++) {
		balls.push_back(new Node("Ball" + QString().setNum(i+1)));
	}

	nRot->addProperty(new IProperty());
//	nRot->addProperty(new DummyUpdatable());
//	nQt->addProperty(new QtLogo(engine.getGLW_TEMPORARY()));
//	nQt->addProperty(new Grid(1.0f, 1.0f, 40, 40));
	//nDuckGrid->addProperty(new Grid(1.0f, 1.0f, 40, 40));
	nCam->addProperty(new Camera(70,1,200));
	nCamFollow->addProperty(new Camera(90,1,200));

	nHead->addProperty(new DummyUpdatable());
	Light* light;
	light = new Light(shadow_mapping);
	light->setDiffuseColor(Vector4f(1.0,0.2,0.2,1.0));
	light->setSpecularColor(Vector4f(1.0,0.2,0.2,1.0));
	nLight->addProperty(light);
	light = new Light(shadow_mapping);
	light->setDiffuseColor(Vector4f(0.2,1.0,0.2,1.0));
	light->setSpecularColor(Vector4f(0.2,1.0,0.2,1.0));
	nLight2->addProperty(light);

	Mesh plane = MESH_MANAGER.get("Plane");
	Mesh cube = MESH_MANAGER.get("Cube");
	Mesh sphere = MESH_MANAGER.get("Sphere_16_32");
	Mesh mesh = MESH_MANAGER.get("duck");

	Material duck = MATERIAL_MANAGER.get("duck");
	Material star = MATERIAL_MANAGER.get("star");
	Material gargoyle = MATERIAL_MANAGER.get("gargoyle");
	Material sand = MATERIAL_MANAGER.get("sand");

	Sample sample = SAMPLE_MANAGER.get("quacking.wav");

	nDuck->addProperty(new MeshRenderer(mesh,duck));
	nStar->addProperty(new MeshRenderer(plane,star));
	//nUni->addProperty(new MeshRenderer(cube,unicorn));
	nGarg->addProperty(new MeshRenderer(cube,gargoyle));
	nSand->addProperty(new MeshRenderer(cube,sand));
	nDuck->addProperty(new DummyControlable());
	nDuck->addProperty(new SoundEmitter(sample));
	nDuck->link(nCamFollow);
	nDuck->link(nHead);
	//nDuck->link(nDuckGrid);

	nHead->link(nStar);
	nHead->link(nLight);
	nHead->link(nLight2);
	nHead->link(nUni);

	sg->link(nDuck);
	sg->link(nRot);
	sg->link(nQt);
	sg->link(nSand);
	for(QVector<Node*>::iterator it = walls.begin() ; it != walls.end() ; it++) {
		sg->link(*it);
	}
	for(QVector<Node*>::iterator it = balls.begin() ; it != balls.end() ; it++) {
		sg->link(*it);
	}
	nRot->link(nCam);
	nRot->link(nGarg);

	nRot->moveTo(Vector3f(0,0,0));
	nQt->moveTo(Vector3f(0,-0.5,0));
	nQt->rotate(Vector3f(1,0,0),90);
	nCam->moveTo(Vector3f(0,10.0,0));
	nCam->rotate(Vector3f(1,0,0),270);
	nCamFollow->moveTo(Vector3f(0,2,3));
	nCamFollow->rotate(Vector3f(1,0,0),345);

	nUni->rotate(Vector3f(0,0,-1),90);
	nUni->rotate(Vector3f(-1,0,0),90);
	nHead->moveTo(Vector3f(0,0.5,0));
	nUni->moveTo(Vector3f(1.5,0,0));
	nStar->moveTo(Vector3f(-1.5,0,0));
	nStar->rotate(Vector3f(0,0,1),270);
	nLight->moveTo(Vector3f(-1.5,0,0));
	nLight2->moveTo(Vector3f(0,0,-3));

	nSand->moveTo(Vector3f(0,-0.5,0));
	nSand->rotate(Vector3f(1,0,0),270);
	nSand->setScale(Vector3f(10,10,0.01));
	walls[0]->moveTo(Vector3f(0,1,-2));
	//walls[0]->moveTo(Vector3f(0,1,-5));
	walls[0]->setScale(Vector3f(10,3,0.01));
	walls[1]->moveTo(Vector3f(0,1, 2));
	//walls[1]->moveTo(Vector3f(0,1, 5));
	walls[1]->setScale(Vector3f(10,3,0.01));
	walls[2]->moveTo(Vector3f( 2,1,0));
	//walls[2]->moveTo(Vector3f( 5,1,0));
	walls[2]->setScale(Vector3f(0.01,3,10));
	walls[3]->moveTo(Vector3f(-2,1,0));
	//walls[3]->moveTo(Vector3f(-5,1,0));
	walls[3]->setScale(Vector3f(0.01,3,10));
	for(int i=0 ; i<balls.size() ; i++) {
		balls[i]->moveTo(Vector3f(0,4+2*i,0));

		float s1 = 0.1+(qrand() % 900)/1000.0;
		float s2 = 0.1+(qrand() % 900)/1000.0;
		float s3 = 0.1+(qrand() % 900)/1000.0;
		balls[i]->setScale(Vector3f(s1,s2,s3));
	}
	nGarg->moveTo(Vector3f(-3,1,-3));
	nGarg->setScale(Vector3f(1,3,1));
	nDuck->moveTo(Vector3f(0,-0.5,-1.8));
	nDuck->rotate(Vector3f(0,1,0),127);
	//nDuckGrid->rotate(Vector3f(1,0,0),270);

	PhysicObject::Properties prop;
	nSand->addProperty(new PhysicObject(prop));
	for(QVector<Node*>::iterator it=walls.begin() ; it != walls.end() ; it++) {
		(*it)->addProperty(new PhysicObject(prop));
		//(*it)->addProperty(new MeshRenderer(cube,gargoyle));
	}
	prop.is_kinematic = true;
	nGarg->addProperty(new PhysicObject(prop));
	prop.shape = PhysicObject::MESH;
	prop.mesh_name = "duck";
	nDuck->addProperty(new PhysicObject(prop));

	prop.is_kinematic = false;
	prop.mass = 100.0;
	prop.restitution = 0.1;
	bool first = true;
	//prop.shape = PhysicObject::SPHERE;
	for(QVector<Node*>::iterator it=balls.begin() ; it != balls.end() ; it++) {
		(*it)->addProperty(new PhysicObject(prop));
		prop.mass += 1.0;
		(*it)->addProperty(new MeshRenderer(mesh,duck));
		if(first) {
			light = new Light(shadow_mapping);
			light->setDiffuseColor(Vector4f(0.2,0.2,1.0,1.0));
			light->setSpecularColor(Vector4f(0.2,0.2,1.0,1.0));
			(*it)->addProperty(light);
			first = false;
		}
	}

	// Beurk ! Mais je peux le faire alors je me prive pas ^^
	//RENDER_MANAGER.setCurrentCamera(static_cast<Camera*>(nCamFollow->properties().child("Camera")));
	RENDER_MANAGER.setCurrentCamera(static_cast<Camera*>(nCamFollow->properties().child("Camera")));
	RenderManager::Background background;
	background.type = RenderManager::SKYBOX;
	background.color = Vector3f(1,1,0);
	background.textures[0] = TEXTURE_MANAGER.get("stormy_front.tga");
	background.textures[1] = TEXTURE_MANAGER.get("stormy_left.tga");
	background.textures[2] = TEXTURE_MANAGER.get("stormy_back.tga");
	background.textures[3] = TEXTURE_MANAGER.get("stormy_right.tga");
	background.textures[4] = TEXTURE_MANAGER.get("stormy_top.tga");
	background.textures[5] = TEXTURE_MANAGER.get("stormy_bottom.tga");
	RENDER_MANAGER.setBackground(background);

	COMMAND_MANAGER.registerCommand("spawn_duck",spawn_duck);
	COMMAND_MANAGER.readFile("config.cfg");

	int ret = engine.start();

	return ret;
}
