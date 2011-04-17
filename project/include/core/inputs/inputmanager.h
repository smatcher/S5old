#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "core/inputs/qtkeytranslator.h"
#include <QMap>

#include "core/utils/singleton.h"

class InputManager
{
public:

	enum InputSource
	{
		Source_Mouse,
		Source_KeyBoard,
		Source_Joystick1,
		Source_Joystick2,
		Source_Joystick3,
		Source_Joystick4,
		INVALID_SOURCE
	};

	enum ControllerType
	{
		Type_Mouse,
		Type_Keyboard,
		Type_Joystick
	};

	enum ButtonStatus
	{
		Pressed,
		Released,
		Held,
		Off
	};

	struct Control
	{
		bool isAxis;
		QString name;
		int id;

		Control(bool isAxis_, const QString& name_) : isAxis(isAxis_), name(name_), id(-1) {}
	};

	struct ControlStatus
	{
		Control control;
		float axisValue;
		ButtonStatus buttonStatus;

		ControlStatus(const Control& ctrl) : control(ctrl), axisValue(0.0), buttonStatus(Off) {}
	};

	InputManager();

	// Register controls
	void addControls(QList<Control> controls);

	// Edit bindings
	void parseBindings(const QString& file);
	void addBinding(const QString& input, const QString& control);
	void removeBinding(const QString& input);

	// Report inputs
	void reportbutton(InputSource source, ButtonStatus status, int button_id);
	void reportaxis(InputSource source, float status, int axis_id);

	// Query controls
	int getControlId(const QString& control);
	float getAxisValue(int id);
	ButtonStatus getButtonStatus(int id);

private:
	QtKeyTranslator m_translator;

	QMap<QString, Control> m_controls_dict;
	QList<ControlStatus> m_controls;
	QList< QMap<int, Control> > m_bindings;

	InputSource extractSource(const QString& input);
};

typedef Singleton<InputManager> INPUT_MANAGER;

#endif // INPUTMANAGER_H
