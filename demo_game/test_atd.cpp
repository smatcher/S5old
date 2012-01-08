#include "core/framework/engine.h"
#include "core/scenegraph/scenegraph.h"
#include "core/scenegraph/node.h"
#include "core/maths/trigo.h"
#include "core/properties/dummyupdatable.h"
#include "core/properties/camera.h"
#include "core/properties/cubemapcamera.h"
#include "core/properties/light.h"
#include "core/managers/rendermanager.h"
#include "core/resources/managers.h"
#include "core/properties/meshrenderer.h"

int main(int argc, char* argv[])
{
	Engine engine(argc, argv,"atd_media/");

	SceneGraph* sg = engine.getScenegraph_TEMPORARY();

	Node* taxi = new Node("Taxi");
	Node* taxi2 = new Node("Taxi2");
	Node* ground = new Node("Ground");
	Node* tiles = new Node("tiles");
	Node* light1 = new Node("Light1");
	Node* light2 = new Node("Light2");
	Node* light3 = new Node("Light3");
	Node* light4 = new Node("Light4");

	sg->link(taxi);
	sg->link(taxi2);
	sg->link(tiles);
	taxi->link(light1);
	taxi->link(light2);
	taxi2->link(light3);
	taxi2->link(light4);
	sg->link(ground);

	Mesh taxi_mesh = MESH_MANAGER.get("taxi");
	Mesh plane = MESH_MANAGER.get("Plane");
	Mesh tiles_mesh = MESH_MANAGER.get("tiles");
	Material taxi_material = MATERIAL_MANAGER.get("taxi_material");
	Material ground_material = MATERIAL_MANAGER.get("ground_material");
	Material bat_material = MATERIAL_MANAGER.get("bat1_material");

	tiles->addProperty(new MeshRenderer(tiles_mesh,bat_material));
	tiles->moveTo(Vector3f(0,-0.5,0));
	tiles->setScale(Vector3f(10,10,10));
	tiles->rotate(Vector3f(1,0,0),-90);

	taxi->addProperty(new MeshRenderer(taxi_mesh,taxi_material));
	taxi->addProperty(new DummyUpdatable());
	taxi->moveTo(Vector3f(3,0,3));
	taxi->rotate(Vector3f(0,1,0),-40);

	taxi2->addProperty(new MeshRenderer(taxi_mesh,taxi_material));
	taxi2->moveTo(Vector3f(0,-0.1,0.0));

	//ground->addProperty(new MeshRenderer(plane, ground_material));
	ground->moveTo(Vector3f(0,-0.5,0));
	ground->setScale(Vector3f(10,10,10));
	ground->rotate(Vector3f(1,0,0),-90);

	Light* lightProp = new Light(true);
	lightProp->setType(Light::SPOT);
	light1->addProperty(lightProp);
	lightProp = new Light(true);
	lightProp->setType(Light::SPOT);
	light2->addProperty(lightProp);
	lightProp = new Light(false);
	lightProp->setType(Light::SPOT);
	light3->addProperty(lightProp);
	lightProp = new Light(false);
	lightProp->setType(Light::SPOT);
	light4->addProperty(lightProp);
	light1->moveTo(Vector3f(-0.85,-0.1,-0.3));
	light2->moveTo(Vector3f(-0.85,-0.1, 0.3));
	light3->moveTo(Vector3f(-0.85,-0.1,-0.3));
	light4->moveTo(Vector3f(-0.85,-0.1, 0.3));

	RenderManager::Background background;
	background.type = RenderManager::Background::COLOR;
	background.color = Vector3f(0.0,0.0,0.0);
	RENDER_MANAGER.setBackground(background);

	return engine.start();
}
