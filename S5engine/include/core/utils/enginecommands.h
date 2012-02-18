#ifndef ENGINECOMMANDS_H
#define ENGINECOMMANDS_H

#include "core/framework/engine.h"
#include "core/managers/commandmanager.h"
#include "core/inputs/inputmanager.h"
#include "core/resources/managers.h"

Engine* g_engine_instance = NULL;

bool bind_key(QStringList);
bool exit_engine(QStringList);
bool toggle_pause_engine(QStringList);
bool take_screenshot(QStringList);
bool renderer_command(QStringList);
bool texture_dbg(QStringList);
bool clr_texture_dbg(QStringList);

void registerEngineCommands(Engine* engine)
{
	g_engine_instance = engine;

	COMMAND_MANAGER.registerCommand("bind",bind_key,"bind a key to a control. Usage is \"bind key control\"");
	COMMAND_MANAGER.registerCommand("exit",exit_engine,"leaves the program");
	COMMAND_MANAGER.registerCommand("pause",toggle_pause_engine,"pauses/unpauses the game");
	COMMAND_MANAGER.registerCommand("renderer",renderer_command,"followed by shadows or pipeline and an appropriate value, changes the renderer options");
	COMMAND_MANAGER.registerCommand("screenshot",take_screenshot,"takes a screenshot, you can specify a path for the screenshot");
	COMMAND_MANAGER.registerCommand("texture_dbg",texture_dbg,"displays the specified texture in the corner");
	COMMAND_MANAGER.registerCommand("clr_texture_dbg",clr_texture_dbg,"remove the textures in the corner");
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

bool renderer_command(QStringList args)
{
	if(args.count() > 2) {
		if(args.at(1) == "shadows") {
			if(args.at(2).toLower() == "true" || args.at(2) == "1") {
				RENDER_MANAGER.setShadowsEnabled(true);
				return TRUE;
			}
			else if(args.at(2).toLower() == "false" || args.at(2) == "0") {
				RENDER_MANAGER.setShadowsEnabled(false);
				return TRUE;
			}
		}
		else if(args.at(1) == "bloom") {
			if(args.at(2).toLower() == "true" || args.at(2) == "1") {
				RENDER_MANAGER.setBloomEnabled(true);
				return TRUE;
			}
			else if(args.at(2).toLower() == "false" || args.at(2) == "0") {
				RENDER_MANAGER.setBloomEnabled(false);
				return TRUE;
			}
		}
		else if(args.at(1) == "pipeline") {
			if(args.at(2).toLower() == "deferred") {
				RENDER_MANAGER.setRenderPipeline(RenderManager::DEFERRED_PIPELINE);
				return TRUE;
			}
			else if(args.at(2).toLower() == "forward") {
				RENDER_MANAGER.setRenderPipeline(RenderManager::FORWARD_PIPELINE);
				return TRUE;
			}
		}
	}

	return FALSE;
}

bool texture_dbg(QStringList args)
{
	if(args.count() > 1) {
		Texture texture = TEXTURE_MANAGER.get(args.at(1));
		if(texture.isValid())
		{
			RENDER_MANAGER.addDebugTexture(texture);
			return TRUE;
		}
	}
	return FALSE;
}

bool clr_texture_dbg(QStringList)
{
	RENDER_MANAGER.clearDebugTextures();
	return TRUE;
}

#endif // ENGINECOMMANDS_H
