#include "debug/tools/debugcamera.h"
#include "core/maths/transform.h"

const double DebugCamera::COEFF_ROTATION = 1.0;
const double DebugCamera::COEFF_TRANSLATION = 0.05;
const double DebugCamera::ZOOM_FACTOR = 1.2;
const bool DebugCamera::ENABLE_RESET = true;	// si ENABLE_RESET=true, alors un appui sur 'R' replace la caméra.
//const double DebugCamera::X_ROTATION_CENTER = 0.0;
//const double DebugCamera::Y_ROTATION_CENTER = 0.0;
//const double DebugCamera::Z_ROTATION_CENTER = 3.0;
const double DebugCamera::DEFAULT_ROTATION_X = 30.0;
const double DebugCamera::DEFAULT_ROTATION_Y = -45.0;
const double DebugCamera::DEFAULT_TRANSLATION_X = 0.0;
const double DebugCamera::DEFAULT_TRANSLATION_Y = 0.0;
const double DebugCamera::DEFAULT_TRANSLATION_Z = -10.0;

// Constructeur
DebugCamera::DebugCamera()
: zoom(1.0), mode(MODE_NONE), original_mouse_x(0), original_mouse_y(0),
rotation_x(DEFAULT_ROTATION_X), rotation_y(DEFAULT_ROTATION_Y),
original_rotation_x(rotation_x), original_rotation_y(rotation_y),
translation_x(DEFAULT_TRANSLATION_X), translation_y(DEFAULT_TRANSLATION_Y), translation_z(DEFAULT_TRANSLATION_Z),
original_translation_x(translation_x), original_translation_y(translation_y), original_translation_z(translation_z)
{
}

// Destructeur
DebugCamera::~DebugCamera()
{
}

// Mise à jour de la caméra
void DebugCamera::update(bool left_pressed, bool right_pressed, bool middle_pressed,
						 int x, int y)
{
	// Zoom :
	/*
	static int previous_mouse_wheel = 0;
	int mouse_wheel = glfwGetMouseWheel();
	int delta = mouse_wheel - previous_mouse_wheel;

	if(delta > 0)
		zoom *= ZOOM_FACTOR*double(delta);
	else if(delta < 0)
		zoom /= ZOOM_FACTOR*double(-delta);

	previous_mouse_wheel = mouse_wheel;

	// Touche 'R' => Reset
	static bool r_previously_pressed = false;
	if(ENABLE_RESET)
	{
		bool r_pressed = glfwGetKey('R') == GLFW_PRESS;
		if(!r_pressed && r_previously_pressed)
			reset();
		r_previously_pressed = r_pressed;
	}
*/
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
		else if(right_pressed)	// Passage au mode TRANSLATION XZ :
		{
			// Enregistrement de l'état actuel (début de la translation)
			mode = MODE_TRANSLATION_XZ;
			original_mouse_x = x;
			original_mouse_y = y;
		}
		else if(middle_pressed)	// Passage au mode TRANSLATION Y :
		{
			// Enregistrement de l'état actuel (début de la translation)
			mode = MODE_TRANSLATION_Y;
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

	// MODE TRANSLATION XZ :
	if(mode == MODE_TRANSLATION_XZ)
	{
		// Bouton appuyé => on met à jour de combien on translate
		if(right_pressed)
		{
			// ATTEMPT 2
			/*
			glLoadIdentity();
			lookAt();

			double p_proj[16];
			glGetDoublev(GL_PROJECTION_MATRIX,p_proj);

			double p_model[16];
			glGetDoublev(GL_MODELVIEW_MATRIX,p_model);

			Transformd scene_to_screen(Matrix4d(p_proj)*Matrix4d(p_model));
			Vector3d imagex=scene_to_screen.apply(Vector3d(1,0,0));
			Vector3d imagey=scene_to_screen.apply(Vector3d(0,1,0));
			Vector3d imagez=scene_to_screen.apply(Vector3d(0,0,1));

			Vector3d delta_mouse((original_mouse_x - x),(y - original_mouse_y),0.0);

			target.x=original_target.x+COEFF_TRANSLATION*imagex*delta_mouse;
			target.y=original_target.y+COEFF_TRANSLATION*imagey*delta_mouse;
			target.z=original_target.z+COEFF_TRANSLATION*imagez*delta_mouse;
			*/

			// ATTEMPT 1

			Transformd temp;
			temp.rotate(Vector3d(0,1,0),-rotation_y);
			//temp.rotate(Vector3d(1,0,0),-rotation_x);
			target=original_target+COEFF_TRANSLATION*temp.apply(Vector3d((original_mouse_x - x),0.0,(original_mouse_y - y)));


			//target.x = original_target.x - COEFF_TRANSLATION*(x - original_mouse_x);
			//target.z = original_target.z - COEFF_TRANSLATION*(y - original_mouse_y);

		}
		// Bouton relâché : on enregistre nos modifs et on repasse au mode NONE
		else
		{
			original_target = target;
			mode = MODE_NONE;
		}
		return;
	}

	// MODE TRANSLATION Y :
	if(mode == MODE_TRANSLATION_Y)
	{
		// Bouton appuyé => on met à jour de combien on translate
		if(middle_pressed)
		{
			target.y = original_target.y - COEFF_TRANSLATION*(original_mouse_y - y);
		}
		// Bouton relâché : on enregistre nos modifs et on repasse au mode NONE
		else
		{
			original_target = target;
			mode = MODE_NONE;
		}
		return;
	}
}

// Mise à jour de la matrice modelview
void DebugCamera::lookAt()
{
	// DEBUG DISPLAY
	/*
	Transformd temp;
	temp.rotate(Vector3d(0,1,0),-rotation_y);
	temp.rotate(Vector3d(1,0,0),-rotation_x);
	Vector3d imagex=temp.apply(Vector3d(1,0,0));
	Vector3d imagey=temp.apply(Vector3d(0,1,0));
	glPointSize(10);
	*/

	glTranslated(translation_x/zoom, translation_y/zoom, translation_z/zoom);
	glRotated(rotation_x, 1.0, 0.0, 0.0);
	glRotated(rotation_y, 0.0, 1.0, 0.0);

	// DEBUG DISPLAY
	/*
	glBegin(GL_POINTS);
		glColor3f(1,0,0); glVertex3d(imagex.x,imagex.y,imagex.z);
		glColor3f(0,1,0); glVertex3d(imagey.x,imagey.y,imagey.z);
		glColor3f(1,1,1); glVertex3d(0,0,0);
	glEnd();

	glBegin(GL_LINES);
		glColor3f(1,0,0); glVertex3d(imagex.x,imagex.y,imagex.z);
		glColor3f(1,1,1); glVertex3d(0,0,0);
	glEnd();

	glBegin(GL_LINES);
		glColor3f(0,1,0); glVertex3d(imagey.x,imagey.y,imagey.z);
		glColor3f(1,1,1); glVertex3d(0,0,0);
	glEnd();
	*/

	glTranslated(-target.x, -target.y, -target.z);

}

// Retour aux valeurs par défaut
void DebugCamera::reset()
{
	zoom = 1.0;

	original_target=target=Vector3f(0,0,0);

	original_rotation_x = rotation_x = DEFAULT_ROTATION_X;
	original_rotation_y = rotation_y = DEFAULT_ROTATION_Y;

	original_translation_x = translation_x = DEFAULT_TRANSLATION_X;
	original_translation_y = translation_y = DEFAULT_TRANSLATION_Y;
	original_translation_z = translation_z = DEFAULT_TRANSLATION_Z;
}

// Choix des valeurs
void DebugCamera::setTranslation(double x, double y, double z)
{
	original_translation_x = translation_x = x;
	original_translation_y = translation_y = y;
	original_translation_z = translation_z = z;
}

void DebugCamera::setRotation(double rot_x, double rot_y)
{
	original_rotation_x = rotation_x = rot_x;
	original_rotation_y = rotation_y = rot_y;
}
