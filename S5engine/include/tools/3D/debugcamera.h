#ifndef DEBUGCAMERA_H
#define DEBUGCAMERA_H

#include "core/maths/vector2.h"
#include "core/maths/vector3.h"

#include "core/graphics/viewpoint.h"

class DebugCamera : public Viewpoint
{
public :

	enum Key
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		RCTRL,
		RSHIFT
	};

protected:
	static const double COEFF_ROTATION;
	static const double COEFF_TRANSLATION;
	static const double DEFAULT_ROTATION_X;
	static const double DEFAULT_ROTATION_Y;

	double zoom;
	Vector3d target;
	Vector3d original_target;

	enum Mode
	{
		MODE_NONE,
		MODE_ROTATION,	// avec le bouton gauche
//		MODE_TRANSLATION_XZ,	// avec le bouton droit
//		MODE_TRANSLATION_Y	// avec le bouton du milieu
	} mode;


	bool keys[6];

	int original_mouse_x;	// Position de la souris au moment où le dragging commence
	int original_mouse_y;

	double rotation_x;	// Rotation actuelle de la scène
	double rotation_y;

	double original_rotation_x;	// Valeur de la rotation au moment où le dragging commence
	double original_rotation_y;

public:
	// Constructeur
	DebugCamera();

	// Destructeur
	virtual ~DebugCamera();

	// Mise à jour de la caméra
	void updateMouse(bool left_pressed, bool right_pressed, bool middle_pressed,
						 int x, int y);

	void updateKeyboard(Key key, bool pressed);

	virtual Vector3f getWorldPosition();
	virtual Frustum getFrustum(int projection_nb);
	virtual IRD::Viewport getViewport(int projection_nb);

	// Mise à jour de la matrice modelview
	virtual int getNbProjections();
	virtual Viewpoint::Style getStyle();
	virtual const Matrix4d& getProjection(double aspect, int projection_nb);
	virtual void setProjection(double aspect, double scale, int projection_nb);

	virtual void applyTransform(int projection_nb);
	virtual void applyOnlyRotation(int projection_nb);

	// Retour aux valeurs par défaut
	void reset();

	// Choix des valeurs
	void setRotation(double rot_x, double rot_y);
	void setRotation(const Vector2d& v) {setRotation(v.x, v.y);}
};


#endif // DEBUGCAMERA_H
