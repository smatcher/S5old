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
	Engine engine(argc, argv,"demo_game/media/");

	SceneGraph* sg = engine.getScenegraph_TEMPORARY();

	Node* head = new Node("Head");
	Node* rot = new Node("Rot");
	Node* light = new Node("Light");

	sg->link(head);
	sg->link(rot);
	rot->link(light);

	Mesh head_mesh = MESH_MANAGER.get("head");
	Material head_material = MATERIAL_MANAGER.get("head");

	head->addProperty(new MeshRenderer(head_mesh,head_material));
	head->setScale(Vector3f(0.3f,0.3f,0.3f));
	head->rotate(Vector3f(1,0,0),270);
	head->rotate(Vector3f(0,1,0),270);

	Light* lightProp = new Light(true);
	lightProp->setType(Light::OMNI);
	light->addProperty(lightProp);
	light->moveTo(Vector3f(-2,2,-2));

	rot->addProperty(new DummyUpdatable(1.0f));

	RenderManager::Background background;
	background.type = RenderManager::Background::COLOR;
	background.color = Vector3f(0.0,0.0,0.0);
	RENDER_MANAGER.setBackground(background);
	RENDER_MANAGER.setAmbient(Vector3f(0.2,0.2,0.2));
	RENDER_MANAGER.setRenderPipeline(RenderManager::FORWARD_PIPELINE);

	//RENDER_MANAGER.addDebugTexture(TEXTURE_MANAGER.get("SSS")); // Texture does not exist at this point

	return engine.start();
}
