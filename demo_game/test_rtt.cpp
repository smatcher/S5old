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

	Node* nCam = new Node("Camera");
	Node* nCubeCam = new Node("CubemapCamera");
	Node* nRot = new Node("Rot");
	Node* nLight = new Node("Light");
	Node* nRttCube = new Node("RTTCube");
	Node* nRttSphere = new Node("RTTSphere");
	Node* nRttL0 = new Node("RTTLight0");

	CubemapCamera* cubecam = new CubemapCamera(1,200);
	Camera* cam = new Camera(100,1,200);
	nCam->addProperty(cam);
	nCubeCam->addProperty(cubecam);
	nLight->addProperty(new Light());
	nRot->addProperty(new DummyUpdatable(0.5));

	sg->link(nRot);
	sg->link(nRttCube);
	sg->link(nRttSphere);
	nLight->link(nRttL0);

	sg->link(nLight);
	nRot->link(nCam);
	nRttSphere->link(nCubeCam);

	nCam->moveTo(Vector3f(0,0,-5));
	nCam->rotate(Vector3f(0,1,0),180);

	nLight->moveTo(Vector3f(2,0,-1));

	nRttCube->moveTo(Vector3f(0,0,6));
	nRttCube->setScale(Vector3f(18,18,1));

	nRttSphere->moveTo(Vector3f(0,0,0));
	nRttSphere->setScale(Vector3f(3,3,3));

	for(int i=0 ; i< 1 ; i++) {
		QString name("RTT_Light");
		name += QString().setNum(i);
		//new RenderTexture2D(name, 512, 512, GL_DEPTH_COMPONENT, GL_FLOAT);
		//new RenderTextureCubemap(name, 512, 512, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE);
		//new RenderTextureCubemap(name, 512, 512, GL_RGBA, GL_UNSIGNED_BYTE);
		new RenderTextureArray(name, 512, 512, 6, GL_RGBA, GL_UNSIGNED_BYTE);
	}

	cam->createTarget(512,512);
	cubecam->createTarget(512,512);
	Mesh cube = MESH_MANAGER.get("Cube");
	Mesh sphere = MESH_MANAGER.get("Sphere_16_32");
	Material rtt = MATERIAL_MANAGER.get("rtt");
	Material rtt_cube = MATERIAL_MANAGER.get("rtt_cube");
	Material rtt0 = MATERIAL_MANAGER.get("rtt_light0_cube");
	Material rtt_array = MATERIAL_MANAGER.get("rtt_array");
	nRttCube->addProperty(new MeshRenderer(cube,rtt));
	nRttSphere->addProperty(new MeshRenderer(sphere, rtt_cube));
	nRttL0->addProperty(new MeshRenderer(sphere,rtt_array));

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

	GLint maxVertexuniforms;
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS,&maxVertexuniforms);
	logInfo(maxVertexuniforms << "maximum vertex uniforms");
	logInfo(GL_PROJECTION_STACK_DEPTH << "maximum projections");

	int ret = engine.start();

	return ret;
}
