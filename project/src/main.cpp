#include <QApplication>

#include "core/framework/engine.h"
#include "core/scenegraph/scenegraph.h"
#include "core/scenegraph/node.h"
#include "core/maths/trigo.h"
#include "core/properties/dummyupdatable.h"
#include "core/properties/dummycontrolable.h"
#include "core/properties/qtlogo.h"
#include "core/properties/camera.h"
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

	nRot->addProperty(new IProperty());
	nRot->addProperty(new DummyUpdatable());
	nQt->addProperty(new QtLogo(engine.getGLW_TEMPORARY()));
	nQt->addProperty(new Grid(1.0f, 1.0f, 40, 40));
	nCam->addProperty(new Camera(70,1,200));
	nCamFollow->addProperty(new Camera(90,1,200));

	Mesh mesh = MESH_MANAGER.get("duckmesh");
	Material material = MATERIAL_MANAGER.get("duckmesh");
	Texture texture = TEXTURE_MANAGER.get("duck.tga");
	Sample sample = SAMPLE_MANAGER.get("quacking.wav");


	Node* nDuck = new Node("Duck");
	nDuck->addProperty(new MeshRenderer(mesh,material,texture));
	nDuck->addProperty(new DummyControlable());
	nDuck->addProperty(new SoundEmitter(sample));
	nDuck->link(nCamFollow);

	sg->link(nDuck);
	sg->link(nRot);
	sg->link(nQt);
	nRot->link(nCam);

	nRot->moveTo(Vector3d(0,0,0));
	nQt->moveTo(Vector3d(0,-0.5,0));
	nQt->rotate(Vector3d(1,0,0),90);
	nCam->moveTo(Vector3d(0,1,2));
	nCam->rotate(Vector3d(1,0,0),330);
	nCamFollow->moveTo(Vector3d(0,2,3));
	nCamFollow->rotate(Vector3d(1,0,0),345);

	int ret = engine.start();

	return ret;
}
