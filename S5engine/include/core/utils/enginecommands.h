#ifndef ENGINECOMMANDS_H
#define ENGINECOMMANDS_H

#include "core/framework/engine.h"
#include "core/managers/commandmanager.h"
#include "core/inputs/inputmanager.h"

Engine* g_engine_instance = NULL;

bool bind_key(QStringList);
bool exit_engine(QStringList);
bool toggle_pause_engine(QStringList);
bool take_screenshot(QStringList);

void registerEngineCommands(Engine* engine)
{
	g_engine_instance = engine;

	COMMAND_MANAGER.registerCommand("bind",bind_key,"bind a key to a control. Usage is \"bind key control\"");
	COMMAND_MANAGER.registerCommand("exit",exit_engine,"leaves the program");
	COMMAND_MANAGER.registerCommand("pause",toggle_pause_engine,"pauses/unpauses the game");
	COMMAND_MANAGER.registerCommand("screenshot",take_screenshot,"takes a screenshot, you can specify a path for the screenshot");
}

bool bind_key(QStringList args)
{
	if(args.count() >= 3) {
		return INPUT_MANAGER.addBinding(args.at(1),args.at(2));
	}

	return false;
}

bool exit_engine(QStringList)
{
	if(g_engine_instance)
	{
		g_engine_instance->stop();
		return true;
	}

	return false;
}


bool toggle_pause_engine(QStringList)
{
	if(g_engine_instance)
	{
		if(g_engine_instance->isPaused())
			g_engine_instance->setPaused(false);
		else
			g_engine_instance->setPaused(true);
		return true;
	}

	return false;
}

bool take_screenshot(QStringList args)
{
	if(args.count() > 1)
	{
		QString path = args.at(1);
		if(path.contains(QRegExp("\\.(png|jpg|bmp)$")))
			RENDER_MANAGER.takeScreenshot(path);
		else
			RENDER_MANAGER.takeScreenshot(path+".png");
	}
	else
	{
		RENDER_MANAGER.takeScreenshot();
	}

	return true;
}

#endif // ENGINECOMMANDS_H
