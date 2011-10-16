#include <QApplication>

#include "core/framework/engine.h"
#include "core/scenegraph/scenegraph.h"
#include "core/scenegraph/node.h"
#include "core/maths/trigo.h"
#include "core/properties/dummyupdatable.h"
#include "core/properties/camera.h"
#include "core/properties/cubemapcamera.h"
#include "core/properties/light.h"
#include "core/managers/rendermanager.h"
#ifdef Q_WS_X11
	#include <X11/Xlib.h>
#endif

#include "core/resources/managers.h"
#include "core/properties/meshrenderer.h"
#include <core/graphics/rendertexture.h>
#include <core/graphics/rt2d.h>
#include <core/graphics/rtcubemap.h>
#include <core/graphics/rtarray.h>

int main(int argc, char *argv[])
{
	#ifdef Q_WS_X11
		XInitThreads();
	#endif

	Engine engine(argc, argv,"demo_game/media/");

	SceneGraph* sg = engine.getScenegraph_TEMPORARY();
	/*
	Node* nCam = new Node("Camera");
	Node* nCubeCam = new Node("CubemapCamera");
	Node* nRttCube = new Node("RTTCube");
	Node* nRttSphere = new Node("RTTSphere");
	Node* nRttL0 = new Node("RTTLight0");
	*/
	Node* nRot = new Node("Rot");
	Node* nLight = new Node("Light");
	Node* nCub1 = new Node("Cube1");
	Node* nCub2 = new Node("Cube2");
	Node* nCub3 = new Node("Cube3");
	Node* nCub4 = new Node("Cube4");
	Node* nCub5 = new Node("Cube5");
	Node* nPlane = new Node("Plane");
	Node* nPlane1 = new Node("Plane1");
	Node* nPlane2 = new Node("Plane2");
	Node* nPlane3 = new Node("Plane3");
	Node* nPlane4 = new Node("Plane4");
	Node* nPlane5 = new Node("Plane5");
	Node* nPlane6 = new Node("Plane6");

	/*
	CubemapCamera* cubecam = new CubemapCamera(1,200);
	Camera* cam = new Camera(100,1,200);
	nCam->addProperty(cam);
	nCubeCam->addProperty(cubecam);
	*/
	nRot->addProperty(new DummyUpdatable(0.5));
	nLight->addProperty(new Light());

	/*
	sg->link(nRttCube);
	sg->link(nRttSphere);
	nRot->link(nCam);
	nRttSphere->link(nCubeCam);
	*/

	sg->link(nRot);
	nRot->link(nCub1);
	nRot->link(nCub2);
	nRot->link(nCub3);
	nRot->link(nCub4);
	nRot->link(nCub5);
	sg->link(nPlane);
	sg->link(nPlane1);
	sg->link(nPlane2);
	sg->link(nPlane3);
	sg->link(nPlane4);
	sg->link(nPlane5);
	sg->link(nPlane6);

	//nLight->link(nRttL0);

	sg->link(nLight);

	nLight->moveTo(Vector3f(-2,1,-1));

	/*
	nCam->moveTo(Vector3f(0,0,-5));
	nCam->rotate(Vector3f(0,1,0),180);

	nRttCube->moveTo(Vector3f(0,0,6));
	nRttCube->setScale(Vector3f(18,18,1));

	nRttSphere->moveTo(Vector3f(0,5,0));
	nRttSphere->setScale(Vector3f(3,3,3));
	*/

	nCub1->moveTo(Vector3f(1,0,0));
	nCub2->moveTo(Vector3f(2,-0.5,1));
	nCub3->moveTo(Vector3f(2,-1,-4));
	nCub4->moveTo(Vector3f(-1,0.25,2));
	nCub5->moveTo(Vector3f(-0.25,-0.5,-1.5));
	nPlane->moveTo(Vector3f(0,-1,0));
	nPlane->rotate(Vector3f(1,0,0),270);
	nPlane->setScale(Vector3f(20,20,20));
	nPlane1->moveTo(Vector3f(0,-25,0));
	nPlane1->rotate(Vector3f(1,0,0),270);
	nPlane1->setScale(Vector3f(50,50,50));
	nPlane2->moveTo(Vector3f(0,25,0));
	nPlane2->rotate(Vector3f(1,0,0),90);
	nPlane2->setScale(Vector3f(50,50,50));
	nPlane3->moveTo(Vector3f(0,0,25));
	nPlane3->rotate(Vector3f(1,0,0),180);
	nPlane3->setScale(Vector3f(50,50,50));
	nPlane4->moveTo(Vector3f(0,0,-25));
	nPlane4->setScale(Vector3f(50,50,50));
	nPlane5->moveTo(Vector3f(-25,0,0));
	nPlane5->rotate(Vector3f(0,1,0),90);
	nPlane5->setScale(Vector3f(50,50,50));
	nPlane6->moveTo(Vector3f(25,0,0));
	nPlane6->rotate(Vector3f(0,1,0),270);
	nPlane6->setScale(Vector3f(50,50,50));


	new RenderTexture2D("Shadowmap", 512, 512, GL_RGBA, GL_UNSIGNED_BYTE);
	new RenderTextureArray("Omni_Lightmap", 256, 256, 6, GL_DEPTH_COMPONENT, GL_FLOAT);
	/*
	for(int i=0 ; i< 1 ; i++) {
		QString name("RTT_Light");
		name += QString().setNum(i);
	//	new RenderTextureArray(name, 512, 512, 6, GL_RGBA, GL_FLOAT);
	}
	*/

	/*
	cam->createTarget(512,512);
	cubecam->createTarget(512,512);
	*/

	Mesh cube = MESH_MANAGER.get("Cube");
	Mesh sphere = MESH_MANAGER.get("Sphere_16_32");
	Mesh plane = MESH_MANAGER.get("Plane");
	Material rtt = MATERIAL_MANAGER.get("rtt");
	Material rtt_cube = MATERIAL_MANAGER.get("rtt_cube");
	Material rtt0 = MATERIAL_MANAGER.get("rtt_light0_cube");
	Material rtt_array = MATERIAL_MANAGER.get("rtt_array");
	Material shadow_rec = MATERIAL_MANAGER.get("shadow_receive");
	/*
	nRttCube->addProperty(new MeshRenderer(cube,rtt));
	nRttSphere->addProperty(new MeshRenderer(sphere, rtt_cube));
	nRttL0->addProperty(new MeshRenderer(sphere,rtt_array));
	*/

	nCub1->addProperty(new MeshRenderer(cube,shadow_rec));
	nCub2->addProperty(new MeshRenderer(cube,shadow_rec));
	nCub3->addProperty(new MeshRenderer(cube,shadow_rec));
	nCub4->addProperty(new MeshRenderer(cube,shadow_rec));
	nCub5->addProperty(new MeshRenderer(cube,shadow_rec));
	nPlane->addProperty(new MeshRenderer(plane,shadow_rec));
	nPlane1->addProperty(new MeshRenderer(plane,shadow_rec));
	nPlane2->addProperty(new MeshRenderer(plane,shadow_rec));
	nPlane3->addProperty(new MeshRenderer(plane,shadow_rec));
	nPlane4->addProperty(new MeshRenderer(plane,shadow_rec));
	nPlane5->addProperty(new MeshRenderer(plane,shadow_rec));
	nPlane6->addProperty(new MeshRenderer(plane,shadow_rec));

	RENDER_MANAGER.setDrawDebug(true);
	RenderManager::Background background;
	background.type = RenderManager::SKYBOX;
	background.textures[0] = TEXTURE_MANAGER.get("stormy_front.tga");
	background.textures[1] = TEXTURE_MANAGER.get("stormy_left.tga");
	background.textures[2] = TEXTURE_MANAGER.get("stormy_back.tga");
	background.textures[3] = TEXTURE_MANAGER.get("stormy_right.tga");
	background.textures[4] = TEXTURE_MANAGER.get("stormy_top.tga");
	background.textures[5] = TEXTURE_MANAGER.get("stormy_bottom.tga");
	RENDER_MANAGER.setBackground(background);

	int ret = engine.start();

	return ret;
}
