#ifndef ENGINECOMMANDS_H
#define ENGINECOMMANDS_H

#include "core/framework/engine.h"
#include "core/managers/commandmanager.h"

Engine* g_engine_instance = NULL;

bool exit_engine(QStringList);
bool toggle_pause_engine(QStringList);

void registerEngineCommands(Engine* engine)
{
	g_engine_instance = engine;

	COMMAND_MANAGER.registerCommand("exit",exit_engine);
	COMMAND_MANAGER.registerCommand("pause",toggle_pause_engine);
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

#endif // ENGINECOMMANDS_H
