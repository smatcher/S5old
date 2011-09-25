#include "core/managers/rendermanager.h"
#include "core/framework/glwidget.h"
#include "core/scenegraph/scenegraph.h"
#include "core/properties/irenderable.h"
#include "core/properties/camera.h"
#include "core/managers/lightingmanager.h"
#include "core/managers/physicsmanager.h"
#include "core/properties/light.h"
#include <core/graphics/rendertarget.h>

#include <QtOpenGL>
#include <math.h>

#include "core/log/log.h"

#ifdef WITH_TOOLS
	#include "tools/widgets/renderwidget.h"
	#include "tools/3D/manipulator.h"
#endif

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

RenderManager::RenderManager() : m_context(NULL), m_camera(NULL), m_cameraChanged(true), m_drawDebug(false)
{
	m_defaultBackground.type = NO_CLEAR;
}

RenderManager::~RenderManager()
{
}

void RenderManager::setupProjection(RenderTarget target)
{
	Camera* camera = target.getCamera();
	QSize resizeTo;
	bool needResize = m_context->needResize(&resizeTo);

	if(target.isOnScreen()) {
		int side = qMax(resizeTo.width(), resizeTo.height());
		glViewport((resizeTo.width() - side) / 2, (resizeTo.height() - side) / 2, side, side);
		m_context->isResized();
	} else {
		glViewport(0, 0, target.getWidth(), target.getHeight());
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(camera != NULL)
	{
		camera->setProjection(1); // Pas besoin de passer l'aspect, il a déjà été pris en compte dans le viewport
	}
	else
	{
		gluPerspective(70,1,0.01,1000);
	}

	glMatrixMode(GL_MODELVIEW);
}

/*
{
	QSize resizeTo;
	bool needResize = m_context->needResize(&resizeTo);
	if(needResize || m_cameraChanged)
	{
		if(needResize)
		{
			int side = qMax(resizeTo.width(), resizeTo.height());
			glViewport((resizeTo.width() - side) / 2, (resizeTo.height() - side) / 2, side, side);
			m_context->isResized();
		}

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		if(camera != NULL)
		{
			camera->setProjection(1); // Pas besoin de passer l'aspect, il a déjà été pris en compte dans le viewport
		}
		else
		{
			gluPerspective(70,1,0.01,1000);
		}

		glMatrixMode(GL_MODELVIEW);
	}

	m_cameraChanged = false;
}
*/

void RenderManager::init(GLWidget* context)
{
	m_context = context;

	m_context->qglClearColor(Qt::black);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_MULTISAMPLE);
	static GLfloat lightPosition[4] = { 1.5, 5.0, 7.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

void RenderManager::render(double elapsed_time, SceneGraph* sg)
{
	// Frame Begin
	for(QVector<IRenderable*>::iterator it = registeredManagees.begin();
		it != registeredManagees.end();
		it++) {
		(*it)->frameBegin(elapsed_time);
	}

	// Render
	for(int i=0 ; i<m_rts.length() ; i++) {
		RenderTarget* rt = m_rts[i];

		if(rt != NULL) {
			rt->bindAsTarget();
			renderTarget(sg, *rt);
			rt->releaseAsTarget();
		}
	}

	RenderTarget crt(m_camera, NULL, 0,0, true);
	renderTarget(sg, crt);

	m_context->swapBuffers();

	// Frame End
	for(QVector<IRenderable*>::iterator it = registeredManagees.begin();
		it != registeredManagees.end();
		it++) {
		(*it)->frameEnd();
	}

	// Debug errors
	GLenum error = glGetError();
	if(error != GL_NO_ERROR) {
		const char* msg = (char*)gluErrorString(error);
		logError( QString(msg) );
	}

}

void RenderManager::renderTarget(SceneGraph* sg, RenderTarget target)
{
	Camera* camera = target.getCamera();
	QList<IRenderable*> transparent_renderables;

	if(m_context == NULL) {
		return;
	}

	if(m_defaultBackground.type == SKYBOX) {
		setupProjection(target);
		glLoadIdentity();
		applyBackground(camera);
	} else {
		glLoadIdentity();
		applyBackground(camera);
		setupProjection(target);
		glLoadIdentity();
	}

	if(camera != NULL) {
		camera->applyTransform();
	} else {
		m_context->applyCamera();
	}

	//std::cout<< registeredManagees.count() << " Renderable nodes to render." << std::endl;

	glEnable(GL_LIGHTING);
	for(int index = 0; index < LIGHTING_MANAGER.managees().count(); index++) {
		LIGHTING_MANAGER.managees().at(index)->sendParameters(index);
	}

	for(QVector<IRenderable*>::iterator it = registeredManagees.begin();
		it != registeredManagees.end();
		it++) {
		glPushMatrix();
		if((*it)->isTransparent()){
			transparent_renderables.push_back(*it); // Transparent renderables are deferred for later rendering
		} else {
			(*it)->render(m_context);
		}
		glPopMatrix();
	}

	for(QList<IRenderable*>::iterator it = transparent_renderables.begin();
		it != transparent_renderables.end();
		it++) {
		glPushMatrix();
		(*it)->render(m_context);
		glPopMatrix();
	}

	if(m_drawDebug && target.isOnScreen())	{
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		for(int i=0 ; i<sg->childCount() ; i++) {
			sg->child(i)->drawDebug(m_context,true);
		}
		PHYSICS_MANAGER.debugDraw();
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
	}

	#ifdef WITH_TOOLS
		sg->getManipulator()->draw(m_context);
	#endif

}

void RenderManager::setCurrentCamera(Camera* cam)
{
	m_camera = cam;
	m_cameraChanged = true;

	#ifdef WITH_TOOLS
		CAMERA_MANAGER.getDebugView()->activeCameraChanged(m_camera);
	#endif
}

void RenderManager::setDrawDebug(bool draw)
{
	m_drawDebug = draw;

	#ifdef WITH_TOOLS
		CAMERA_MANAGER.getDebugView()->setDrawDebug(draw);
	#endif
}

const Camera* RenderManager::getCurrentCamera()
{
	return m_camera;
}

void RenderManager::setBackground(const Background &background)
{
	m_defaultBackground = background;
};

void RenderManager::applyBackground(Camera* camera)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glDepthMask(false);

	switch(m_defaultBackground.type)
	{
		case COLOR :
			glClearColor(m_defaultBackground.color.r, m_defaultBackground.color.g, m_defaultBackground.color.b, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			break;
		case SINGLE_TEXTURE :
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
				glLoadIdentity();
				gluOrtho2D(0,1,0,1);

				glDisable(GL_LIGHTING);
				glEnable(GL_TEXTURE_2D);
				glColor4f(1,1,1,1);
				m_defaultBackground.textures[0]->bind();
				glBegin(GL_QUADS);
					glTexCoord2f(0,0); glVertex2f(0,0);
					glTexCoord2f(1,0); glVertex2f(1,0);
					glTexCoord2f(1,1); glVertex2f(1,1);
					glTexCoord2f(0,1); glVertex2f(0,1);
				glEnd();
				glDisable(GL_TEXTURE_2D);
				glEnable(GL_LIGHTING);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			break;
		case SKYBOX :
			glPushMatrix();
				if(camera != NULL)
					camera->applyOnlyRotation();
				else
					m_context->applyCameraRotation();
				glDisable(GL_LIGHTING);
				glEnable(GL_TEXTURE_2D);
				glColor4f(1,1,1,1);

				// Render the front quad
				m_defaultBackground.textures[0]->bind();
				glBegin(GL_QUADS);
					glTexCoord2f(0, 0); glVertex3f(  10, -10, -10 );
					glTexCoord2f(0, 1); glVertex3f(  10,  10, -10 );
					glTexCoord2f(1, 1); glVertex3f( -10,  10, -10 );
					glTexCoord2f(1, 0); glVertex3f( -10, -10, -10 );
				glEnd();

				// Render the left quad
				m_defaultBackground.textures[1]->bind();
				glBegin(GL_QUADS);
					glTexCoord2f(0, 0); glVertex3f(  10, -10,  10 );
					glTexCoord2f(0, 1); glVertex3f(  10,  10,  10 );
					glTexCoord2f(1, 1); glVertex3f(  10,  10, -10 );
					glTexCoord2f(1, 0); glVertex3f(  10, -10, -10 );
				glEnd();

				// Render the back quad
				m_defaultBackground.textures[2]->bind();
				glBegin(GL_QUADS);
					glTexCoord2f(0, 0); glVertex3f( -10, -10,  10 );
					glTexCoord2f(0, 1); glVertex3f( -10,  10,  10 );
					glTexCoord2f(1, 1); glVertex3f(  10,  10,  10 );
					glTexCoord2f(1, 0); glVertex3f(  10, -10,  10 );

				glEnd();

				// Render the right quad
				m_defaultBackground.textures[3]->bind();
				glBegin(GL_QUADS);
					glTexCoord2f(0, 1); glVertex3f( -10,  10, -10 );
					glTexCoord2f(1, 1); glVertex3f( -10,  10,  10 );
					glTexCoord2f(1, 0); glVertex3f( -10, -10,  10 );
					glTexCoord2f(0, 0); glVertex3f( -10, -10, -10 );
				glEnd();

				// Render the top quad
				m_defaultBackground.textures[4]->bind();
				glBegin(GL_QUADS);
					glTexCoord2f(1, 1); glVertex3f(  10,  10, -10 );
					glTexCoord2f(1, 0); glVertex3f(  10,  10,  10 );
					glTexCoord2f(0, 0); glVertex3f( -10,  10,  10 );
					glTexCoord2f(0, 1); glVertex3f( -10,  10, -10 );
				glEnd();

				// Render the bottom quad
				m_defaultBackground.textures[5]->bind();
				glBegin(GL_QUADS);
					glTexCoord2f(0, 0); glVertex3f( -10, -10, -10 );
					glTexCoord2f(0, 1); glVertex3f( -10, -10,  10 );
					glTexCoord2f(1, 1); glVertex3f(  10, -10,  10 );
					glTexCoord2f(1, 0); glVertex3f(  10, -10, -10 );
				glEnd();

				glDisable(GL_TEXTURE_2D);
				glEnable(GL_LIGHTING);
			glPopMatrix();
			break;
		default :
			break;
	}

	glDepthMask(true);
}

void RenderManager::addRenderTarget(RenderTarget *rt)
{
	if(rt != NULL) {
		m_rts.push_back(rt);
	} else {
		logWarn("tried to add NULL render target");
	}
}

