#include "core/managers/rendermanager.h"
#include "core/framework/glwidget.h"
#include "core/scenegraph/scenegraph.h"
#include "core/properties/irenderable.h"
#include "core/properties/camera.h"

#include <QtOpenGL>
#include <math.h>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

RenderManager::RenderManager() : m_context(NULL), m_camera(NULL), m_cameraChanged(true), m_drawDebug(false)
{
}

RenderManager::~RenderManager()
{
}

void RenderManager::setupProjection()
{
	QSize resizeTo;
	bool needResize = m_context->needResize(&resizeTo);
	if(needResize || m_cameraChanged)
	{
		if(needResize)
		{
			int side = qMin(resizeTo.width(), resizeTo.height());
			glViewport((resizeTo.width() - side) / 2, (resizeTo.height() - side) / 2, side, side);
			m_context->isResized();
		}

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		if(m_camera != NULL)
		{
//			Matrix4d mat = m_camera->getProjection((float)resizeTo.width() / (float)resizeTo.height());
//			glLoadMatrixd(mat.values);
			m_camera->setProjection(1); // Pas besoin de passer l'aspect, il a déjà été pris en compte dans le viewport
		}
		else
		{
			#ifdef QT_OPENGL_ES_1
				glOrthof(-5, +5, -5, +5, 1.0, 50.0);
			#else
				glOrtho(-5, +5, -5, +5, 1.0, 50.0);
			#endif
		}

		glMatrixMode(GL_MODELVIEW);
	}

	m_cameraChanged = false;
}

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
}

void RenderManager::render(double elapsed_time, SceneGraph* sg)
{
	if(m_context == NULL)
		return;

	setupProjection();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if(m_camera != NULL)
	{
		m_camera->node()->globalTransform().getInverse().glMultd();
	}
	else
	{
		glTranslatef(0.0, 0.0, -10.0);
		glRotatef(m_context->xRot / 16.0, 1.0, 0.0, 0.0);
		glRotatef(m_context->yRot / 16.0, 0.0, 1.0, 0.0);
		glRotatef(m_context->zRot / 16.0, 0.0, 0.0, 1.0);
	}

	//std::cout<< registeredManagees.count() << " Renderable nodes to render." << std::endl;

	for(int index = 0; index < registeredManagees.count(); index++)
	{
		glPushMatrix();
		IRenderable* prop = (IRenderable*)registeredManagees[index];
		prop->render(elapsed_time, m_context);
		glPopMatrix();
	}

    if(m_drawDebug)
    {
        glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		for(int i=0 ; i<sg->childCount() ; i++)
        {
            sg->child(i)->drawDebug(m_context,true);
        }
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
    }

	m_context->swapBuffers();

	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
	{
	   qDebug() << gluErrorString(error);
	}
}

void RenderManager::setCurrentCamera(Camera* cam)
{
	m_camera = cam;
	m_cameraChanged = true;
}

void RenderManager::setDrawDebug(bool draw)
{
    m_drawDebug = draw;
}
