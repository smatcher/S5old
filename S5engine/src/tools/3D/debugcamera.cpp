#include "tools/3D/debugcamera.h"
#include "core/maths/transform.h"
#include "core/log/log.h"
#include <GL/glu.h>

#include <QDateTime>

const double DebugCamera::COEFF_ROTATION = 0.3;
const double DebugCamera::COEFF_TRANSLATION = 10;
const double DebugCamera::DEFAULT_ROTATION_X = 30.0;
const double DebugCamera::DEFAULT_ROTATION_Y = 135.0;

// Constructeur
DebugCamera::DebugCamera()
: zoom(1.0), mode(MODE_NONE), original_mouse_x(0), original_mouse_y(0),
rotation_x(DEFAULT_ROTATION_X), rotation_y(DEFAULT_ROTATION_Y),
original_rotation_x(rotation_x), original_rotation_y(rotation_y)
{
	for(int i=0 ; i<6 ; i++)
		keys[i] = false;

	target = Vector3f(-3,3,-3);
}

// Destructeur
DebugCamera::~DebugCamera()
{
}

void DebugCamera::updateKeyboard(Key key, bool pressed)
{
	keys[key] = pressed;
}

// Mise à jour de la caméra
void DebugCamera::updateMouse(bool left_pressed, bool right_pressed, bool middle_pressed,
						 int x, int y)
{
	// Cas où l'on passe du mode NONE à un autre :
	if(mode == MODE_NONE)
	{
		if(left_pressed)	// Passage au mode ROTATION :
		{
			// Enregistrement de l'état actuel (début de la rotation)
			mode = MODE_ROTATION;
			original_mouse_x = x;
			original_mouse_y = y;
		}
		return;
	}

	// MODE ROTATION :
	if(mode == MODE_ROTATION)
	{
		// Bouton appuyé => on met à jour de combien on tourne
		if(left_pressed)
		{
			rotation_x = original_rotation_x + COEFF_ROTATION*(y - original_mouse_y);
			rotation_y = original_rotation_y + COEFF_ROTATION*(x - original_mouse_x);
		}
		// Bouton relâché : on enregistre nos modifs et on repasse au mode NONE
		else
		{
			original_rotation_x = rotation_x;
			original_rotation_y = rotation_y;
			mode = MODE_NONE;
		}
		return;
	}
}

int DebugCamera::getNbProjections()
{
	return 1;
}

Viewpoint::Style DebugCamera::getStyle()
{
	return MONO;
}

const Matrix4d& DebugCamera::getProjection(double aspect, int projection_nb)
{
	logWarn("Not implemented");
	Matrix4d mat;
	return mat;
}

void DebugCamera::setProjection(double aspect, double scale, int projection_nb)
{
	gluPerspective(70,aspect,0.01,1000);
	glScaled(scale, scale, 1);
}

// Mise à jour de la matrice modelview
void DebugCamera::applyTransform(int projection_nb)
{
	// Compute elapsed time
	static QDateTime lastTime = QDateTime::currentDateTime();
	QDateTime time = QDateTime::currentDateTime();
	double elapsed = double(lastTime.msecsTo(time))/1000.0;
	lastTime = time;

	// Compute translation
	Transformf temp;
	temp.rotate(Vector3f(1,0,0),-rotation_x);
	temp.rotate(Vector3f(0,1,0),-rotation_y);
	if(keys[UP])
		target = target + COEFF_TRANSLATION*elapsed*temp.apply(Vector3f(0,0,-1));
	if(keys[DOWN])
		target = target + COEFF_TRANSLATION*elapsed*temp.apply(Vector3f(0,0,1));
	if(keys[LEFT])
		target = target + COEFF_TRANSLATION*elapsed*temp.apply(Vector3f(-1,0,0));
	if(keys[RIGHT])
		target = target + COEFF_TRANSLATION*elapsed*temp.apply(Vector3f(1,0,0));
	if(keys[RCTRL])
		target = target + COEFF_TRANSLATION*elapsed*temp.apply(Vector3f(0,-1,0));
	if(keys[RSHIFT])
		target = target + COEFF_TRANSLATION*elapsed*temp.apply(Vector3f(0,1,0));

	// Apply matrix
	glRotated(rotation_x, 1.0, 0.0, 0.0);
	glRotated(rotation_y, 0.0, 1.0, 0.0);

	glTranslated(-target.x, -target.y, -target.z);

}

void DebugCamera::applyOnlyRotation(int projection_nb)
{
	glRotated(rotation_x, 1.0, 0.0, 0.0);
	glRotated(rotation_y, 0.0, 1.0, 0.0);
}

// Retour aux valeurs par défaut
void DebugCamera::reset()
{
	zoom = 1.0;

	original_target=target=Vector3f(0,0,0);

	original_rotation_x = rotation_x = DEFAULT_ROTATION_X;
	original_rotation_y = rotation_y = DEFAULT_ROTATION_Y;
}

void DebugCamera::setRotation(double rot_x, double rot_y)
{
	original_rotation_x = rotation_x = rot_x;
	original_rotation_y = rotation_y = rot_y;
}
