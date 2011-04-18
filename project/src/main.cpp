#include <QApplication>

#include "core/framework/engine.h"
#include "core/scenegraph/scenegraph.h"
#include "core/scenegraph/node.h"
#include "core/maths/trigo.h"
#include "core/properties/dummyupdatable.h"
#include "core/properties/dummycontrolable.h"
#include "core/properties/qtlogo.h"
#include "core/properties/camera.h"
#include "core/managers/rendermanager.h"
#ifdef Q_WS_X11
	#include <X11/Xlib.h>
#endif

#include "core/resources/managers.h"
#include "core/properties/meshrenderer.h"

#include "core/inputs/inputmanager.h"

int main(int argc, char *argv[])
{
	#ifdef Q_WS_X11
		XInitThreads();
	#endif

	Engine engine(argc, argv);

	SceneGraph* sg = engine.getScenegraph_TEMPORARY();

	QList<InputManager::Control> controls;
	controls.push_back(InputManager::Control(false,"avance"));
	controls.push_back(InputManager::Control(false,"recule"));
	controls.push_back(InputManager::Control(false,"gauche"));
	controls.push_back(InputManager::Control(false,"droite"));
	INPUT_MANAGER::getInstance().addControls(controls);

	INPUT_MANAGER::getInstance().addBinding("KB_Z","avance");
	INPUT_MANAGER::getInstance().addBinding("KB_Up","avance");
	INPUT_MANAGER::getInstance().addBinding("KB_S","recule");
	INPUT_MANAGER::getInstance().addBinding("KB_Down","recule");
	INPUT_MANAGER::getInstance().addBinding("KB_Q","gauche");
	INPUT_MANAGER::getInstance().addBinding("KB_Left","gauche");
	INPUT_MANAGER::getInstance().addBinding("KB_D","droite");
	INPUT_MANAGER::getInstance().addBinding("KB_Right","droite");

    Node nRot("Rotating node");
	Node nQt("Qt Logo");
	Node nCam("Camera");

    IProperty prop1;
    DummyUpdatable rot;
	QtLogo qt(engine.getGLW_TEMPORARY());
	Camera cam(70,1,200);

	nRot.properties().link(&prop1);
	nRot.properties().link(&rot);
	nQt.properties().link(&qt);
	nCam.properties().link(&cam);

	Mesh mesh = MeshManager::getInstance().get("duckmesh");
	Material material = MaterialManager::getInstance().get("duckmesh");
	Texture texture = TextureManager::getInstance().get("duck.tga");

	Node nDuck("Duck");
	MeshRenderer mrender(mesh,material,texture);
	DummyControlable cont;
	nDuck.properties().link(&mrender);
	nDuck.properties().link(&cont);

	sg->link(&nDuck);
	sg->link(&nRot);
	sg->link(&nQt);
	nRot.link(&nCam);

    nRot.moveTo(Vector3d(0,0,0));
	nQt.moveTo(Vector3d(0,2,0));
    nCam.moveTo(Vector3d(0,1,2));
	nCam.rotate(Vector3d(1,0,0),330);

	int ret = engine.start();

    nRot.unlinkAll();

	return ret;
}
