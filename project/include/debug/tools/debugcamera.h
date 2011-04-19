#ifndef DEBUGCAMERA_H
#define DEBUGCAMERA_H

#include "core/maths/vector2.h"
#include "core/maths/vector3.h"

class DebugCamera
{
protected:
	static const double COEFF_ROTATION;
	static const double COEFF_TRANSLATION;
	static const double ZOOM_FACTOR;
	static const bool ENABLE_RESET;	// si ENABLE_RESET=true, alors un appui sur 'R' replace la caméra.
	//static const double X_ROTATION_CENTER;
	//static const double Y_ROTATION_CENTER;
	//static const double Z_ROTATION_CENTER;
	static const double DEFAULT_ROTATION_X;
	static const double DEFAULT_ROTATION_Y;
	static const double DEFAULT_TRANSLATION_X;
	static const double DEFAULT_TRANSLATION_Y;
	static const double DEFAULT_TRANSLATION_Z;

	double zoom;
	Vector3d target;
	Vector3d original_target;

	enum Mode
	{
		MODE_NONE,
		MODE_ROTATION,	// avec le bouton gauche
		MODE_TRANSLATION_XZ,	// avec le bouton droit
		MODE_TRANSLATION_Y	// avec le bouton du milieu
	} mode;

	int original_mouse_x;	// Position de la souris au moment où le dragging commence
	int original_mouse_y;

	double rotation_x;	// Rotation actuelle de la scène
	double rotation_y;

	double original_rotation_x;	// Valeur de la rotation au moment où le dragging commence
	double original_rotation_y;

	// Pareil pour la translation...
	double translation_x, translation_y, translation_z;

	double original_translation_x;
	double original_translation_y;
	double original_translation_z;
public:
	// Constructeur
	DebugCamera();

	// Destructeur
	virtual ~DebugCamera();

	// Mise à jour de la caméra
	void update(bool left_pressed, bool right_pressed, bool middle_pressed,
						 int x, int y);

	// Mise à jour de la matrice modelview
	void lookAt();

	// Retour aux valeurs par défaut
	void reset();

	// Choix des valeurs
	void setTranslation(double x, double y, double z);
	void setTranslation(const Vector3d& v) {setTranslation(v.x, v.y, v.z);}

	void setRotation(double rot_x, double rot_y);
	void setRotation(const Vector2d& v) {setRotation(v.x, v.y);}
};


#endif // DEBUGCAMERA_H
