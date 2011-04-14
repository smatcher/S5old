#include <QApplication>

#include "core/framework/engine.h"
#include "core/scenegraph/scenegraph.h"
#include "core/scenegraph/node.h"
#include "core/maths/trigo.h"
#include "core/properties/dummyupdatable.h"
#include "core/properties/qtlogo.h"
#include "core/properties/camera.h"
#include "core/properties/meshrenderer.h"
#include "core/managers/rendermanager.h"
#ifdef Q_WS_X11
	#include <X11/Xlib.h>
#endif

#include "core/resources/managers.h"

int main(int argc, char *argv[])
{
	#ifdef Q_WS_X11
		XInitThreads();
	#endif

	Engine engine(argc, argv);

    SceneGraph* sg = engine.getScenegraph_TEMPORARY();

    Node nRot("Rotating node");
	Node nQt("Qt Logo");
	Node nCam("Camera");
	Node nDuck("Duck");

	Mesh mesh = MeshManager::getInstance().get("duckmesh");
	Material material = MaterialManager::getInstance().get("duckmesh");
	Texture texture = TextureManager::getInstance().get("duck.tga");

    IProperty prop1;
    DummyUpdatable rot;
	QtLogo qt(engine.getGLW_TEMPORARY());
	Camera cam(70,1,200);
	MeshRenderer mrender(mesh,material,texture);

	nRot.properties().link(&prop1);
	nRot.properties().link(&rot);
	nQt.properties().link(&qt);
	nCam.properties().link(&cam);
	nDuck.properties().link(&mrender);

	sg->link(&nRot);
	sg->link(&nQt);
	sg->link(&nDuck);
	nRot.link(&nCam);

    nRot.moveTo(Vector3d(0,0,0));
	nQt.moveTo(Vector3d(0,2,0));
    nCam.moveTo(Vector3d(0,1,2));
	nCam.rotate(Vector3d(1,0,0),330);
	nDuck.moveTo(Vector3d(0,0,0));

	int ret = engine.start();

    nRot.unlinkAll();

	return ret;
}
