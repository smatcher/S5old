#include "core/managers/rendermanager.h"
#include "core/framework/glwidget.h"
#include "core/scenegraph/scenegraph.h"
#include "core/properties/irenderable.h"

#include <QtOpenGL>
#include <math.h>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

RenderManager::RenderManager() : m_context(NULL)
{
}

RenderManager::~RenderManager()
{
}

void RenderManager::init(GLWidget* context)
{
	m_context = context;

	m_context->makeCurrent();
	m_context->qglClearColor(Qt::black);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_MULTISAMPLE);
	static GLfloat lightPosition[4] = { 1.5, 5.0, 7.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	m_context->doneCurrent();
}

void RenderManager::render(double elapsed_time, SceneGraph* sg)
{
	if(m_context == NULL)
		return;

	m_context->makeCurrent();

	QSize resizeTo;
	if(m_context->needResize(&resizeTo))
	{
		int side = qMin(resizeTo.width(), resizeTo.height());
		glViewport((resizeTo.width() - side) / 2, (resizeTo.height() - side) / 2, side, side);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		#ifdef QT_OPENGL_ES_1
			glOrthof(-5, +5, -5, +5, 1.0, 50.0);
		#else
			glOrtho(-5, +5, -5, +5, 1.0, 50.0);
		#endif
		glMatrixMode(GL_MODELVIEW);

		m_context->isResized();
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -10.0);
	glRotatef(m_context->xRot / 16.0, 1.0, 0.0, 0.0);
	glRotatef(m_context->yRot / 16.0, 0.0, 1.0, 0.0);
	glRotatef(m_context->zRot / 16.0, 0.0, 0.0, 1.0);

	//std::cout<< registeredManagees.count() << " Renderable nodes to render." << std::endl;

	for(int index = 0; index < registeredManagees.count(); index++)
	{
		glPushMatrix();
		IRenderable* prop = (IRenderable*)registeredManagees[index];
		prop->render(elapsed_time, m_context);
		glPopMatrix();
	}

	glDisable(GL_LIGHTING);
	for(int i=0 ; i<sg->childCount() ; i++)
	{
		sg->child(i)->drawTransform(m_context,true);
	}
	glEnable(GL_LIGHTING);

	m_context->swapBuffers();
	m_context->doneCurrent();
}
