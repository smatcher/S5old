#include <QApplication>

#include "core/framework/engine.h"
#include "core/scenegraph/scenegraph.h"
#include "core/scenegraph/node.h"
#include "core/maths/trigo.h"
#include "core/properties/dummyupdatable.h"
#include "core/properties/camera.h"
#include "core/properties/light.h"
#include "core/managers/rendermanager.h"
#include "core/animation/skeleton.h"
#include "core/animation/skeletonanimator.h"
#ifdef Q_WS_X11
	#include <X11/Xlib.h>
#endif

#include "core/resources/managers.h"
#include "core/properties/meshrenderer.h"
#include "core/properties/skinnedmeshrenderer.h"

int main(int argc, char *argv[])
{
	#ifdef Q_WS_X11
		XInitThreads();
	#endif

	Engine engine(argc, argv,"demo_game/media/");

	SceneGraph* sg = engine.getScenegraph_TEMPORARY();

	Node* nCam = new Node("Camera");
	Node* nLight = new Node("Light");
	Node* nActor = new Node("Actor1");
	Node* nActor2 = new Node("Actor2");
	Node* nActor3 = new Node("Actor3");
	Node* nRttCube = new Node("RTTCube");

	Camera* cam = new Camera(70,1,200);
	nCam->addProperty(cam);
	nLight->addProperty(new Light());

	sg->link(nActor);
	sg->link(nActor2);
	sg->link(nActor3);
	sg->link(nCam);
	sg->link(nRttCube);

	sg->link(nLight);

	nCam->moveTo(Vector3f(0,5,-5));
	nCam->rotate(Vector3f(0,1,0),180);

	nActor2->move(Vector3f(4,0,0));
	nActor3->move(Vector3f(-4,0,0));

	nLight->moveTo(Vector3f(2,0,-1));

	nRttCube->moveTo(Vector3f(0,7,4));
	nRttCube->setScale(Vector3f(15,15,1));

	#define ANIMATE

	cam->createTarget(512,512);
	Mesh cube = MESH_MANAGER.get("Cube");
	Material rtt = MATERIAL_MANAGER.get("rtt");
	nRttCube->addProperty(new MeshRenderer(cube,rtt));

	SkinnedMeshRenderer* meshrenderer;
	Skeleton* skeleton;

#define USE_DWARF
#define USE_PWIPS
#define USE_BOB

#ifdef USE_BOB
	Mesh bob_mesh = MESH_MANAGER.get("Bob");
	Material bob_material = MATERIAL_MANAGER.get("bob");
	meshrenderer = new SkinnedMeshRenderer(bob_mesh, bob_material);
	nActor->addProperty(meshrenderer);
	skeleton = bob_mesh->getSkeleton();
	if(skeleton != NULL) {
		nActor->link(skeleton->buildSkeleton());
		meshrenderer->createLinks();

		Node* lamp = nActor->find("SKELETON_lamp");
		if(lamp != NULL) {
			lamp->link(nLight);
			nLight->moveTo(Vector3f(0,1,0));
		}

		#ifdef ANIMATE
		SkeletonAnimator* animator = new SkeletonAnimator(skeleton);
		nActor->addProperty(animator);
		animator->createLinks();
		#endif
	}
#endif

#ifdef USE_PWIPS
	Mesh pwips_mesh = MESH_MANAGER.get("pwips");
	Material pwips_material = MATERIAL_MANAGER.get("pwips");
	meshrenderer = new SkinnedMeshRenderer(pwips_mesh, pwips_material);
	nActor2->addProperty(meshrenderer);
	skeleton = pwips_mesh->getSkeleton();
	if(skeleton != NULL) {
		nActor2->link(skeleton->buildSkeleton());
		meshrenderer->createLinks();

		#ifdef ANIMATE
		SkeletonAnimator* animator = new SkeletonAnimator(skeleton);
		nActor2->addProperty(animator);
		animator->createLinks();
		#endif

		nActor2->setScale(Vector3f(0.12,0.12,0.12));
		nActor2->move(Vector3f(0.0,4.0,0.0));
	}
#endif

#ifdef USE_DWARF
	Mesh dwarf_mesh = MESH_MANAGER.get("dwarf");
	Material dwarf_material = MATERIAL_MANAGER.get("dwarf");
	meshrenderer = new SkinnedMeshRenderer(dwarf_mesh, dwarf_material);
	nActor3->addProperty(meshrenderer);
	skeleton = dwarf_mesh->getSkeleton();
	if(skeleton != NULL) {
		nActor3->link(skeleton->buildSkeleton());
		meshrenderer->createLinks();

		#ifdef ANIMATE
		SkeletonAnimator* animator = new SkeletonAnimator(skeleton);
		nActor3->addProperty(animator);
		animator->createLinks();
		#endif

		nActor3->setScale(Vector3f(0.1,0.1,0.1));
		nActor3->rotate(Vector3f(0.0,1.0,0.0),180);
	}
#endif

	RENDER_MANAGER.setDrawDebug(true);
	//RENDER_MANAGER.setCurrentCamera(static_cast<Camera*>(nCam->properties().child("Camera")));
	RenderManager::Background background;
	background.type = RenderManager::COLOR;
	background.color = Vector3f(0.2,0.2,0.2);

	RENDER_MANAGER.setBackground(background);

	GLint maxVertexuniforms;
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS,&maxVertexuniforms);
	logInfo(maxVertexuniforms << "maximum vertex uniforms");
	logInfo(GL_PROJECTION_STACK_DEPTH << "maximum projections");

	int ret = engine.start();

	return ret;
}
