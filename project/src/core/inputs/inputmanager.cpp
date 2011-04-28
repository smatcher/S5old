#include "core/inputs/inputmanager.h"
#include <QStringList>

#include "debug/log/log.h"

InputManager::InputManager()
{
	for(int i=Source_Mouse ; i<=Source_Joystick4 ; i++)
	{
		m_bindings.push_back(QMap<int,Control>());
	}
}

void InputManager::addControl(Control control)
{
	if(!m_controls_dict.contains(control.name))
	{
		control.id = m_controls.length();
		m_controls_dict.insert(control.name, control);
		m_controls.push_back(ControlStatus(control));
	}
	else
	{
		logWarn( "control " << control.name << " declared more than once");
	}
}

void InputManager::addControls(QList<Control> controls)
{
	for(QList<Control>::Iterator it = controls.begin() ; it != controls.end() ; it++)
	{
		addControl(*it);
	}
}

void InputManager::parseBindings(const QString &file)
{
}

void InputManager::addBinding(const QString &input, const QString &control)
{
	InputSource source_index = extractSource(input);

	// If input exists
	if(source_index != INVALID_SOURCE)
	{
		// Find control id
		QMap<QString, Control>::Iterator control_id = m_controls_dict.find(control);

		if(control_id != m_controls_dict.end())
		{
			int input_value = m_translator.toValue(input);

			if(input_value != -1)
			{
				QMap<int,Control>::Iterator input_id = m_bindings[source_index].find(input_value);

				if(input_id != m_bindings[source_index].end())
				{
					logError( input << " already bound to " << input_id.value().name );
				}
				else
				{
					m_bindings[source_index].insert(input_value, *control_id);
				}
			}
			else
			{
				logError( input << " is not a valid key" );
			}
		}
		else
		{
			logError( "cannot bind to control " << control << " because it does not exist");
		}
	}
	else
	{
		logError( "cannot bind " << input << " because it is not a valid key");
	}
}

void InputManager::removeBinding(const QString &input)
{
	InputSource source_index = extractSource(input);

	if(source_index != INVALID_SOURCE)
	{
		// TODO
	}
}

void InputManager::reportButton(InputSource source, ButtonStatus status, int button_id)
{
	// Find binding
	QMap<int, Control>::Iterator control = m_bindings[source].find(button_id);

	if(control != m_bindings[source].end())
		m_controls[control->id].buttonStatus = status;
}

void InputManager::reportAxis(InputSource source, float status, int axis_id)
{
}

int InputManager::getControlId(const QString &control)
{
	// Find control id
	QMap<QString, Control>::Iterator control_id = m_controls_dict.find(control);
	int ret = -1;

	if(control_id != m_controls_dict.end())
	{
		ret = control_id.value().id;
	}
	else
	{
		logError( "trying to access id for " << control << " which is an unknwown control");
	}

	return ret;
}

float InputManager::getAxisValue(int id)
{
	return m_controls[id].axisValue;
}

InputManager::ButtonStatus InputManager::getButtonStatus(int id)
{
	return m_controls[id].buttonStatus;
}

InputManager::InputSource InputManager::extractSource(const QString& input)
{
	// Extract source prefix from input name
	QString prefix = input.split("_")[0];
	InputSource source_index = INVALID_SOURCE;

	// Search input source code
	if(prefix == "KB")
		source_index = Source_KeyBoard;
	else if(prefix == "MS")
		source_index = Source_Mouse;
	else if(prefix == "JS1")
		source_index = Source_Joystick1;
	else if(prefix == "JS2")
		source_index = Source_Joystick2;
	else if(prefix == "JS3")
		source_index = Source_Joystick3;
	else if(prefix == "JS4")
		source_index = Source_Joystick4;

	return source_index;
}

