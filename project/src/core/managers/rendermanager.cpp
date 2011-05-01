#include "core/managers/rendermanager.h"
#include "core/framework/glwidget.h"
#include "core/scenegraph/scenegraph.h"
#include "core/properties/irenderable.h"
#include "core/properties/camera.h"

#include <QtOpenGL>
#include <math.h>

#include "debug/log/log.h"

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

void RenderManager::setupProjection()
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

    if(m_defaultBackground.type == SKYBOX)
    {
        setupProjection();
        glLoadIdentity();
        applyBackground();
    }
    else
    {
        glLoadIdentity();
        applyBackground();
        setupProjection();
        glLoadIdentity();
    }


	if(m_camera != NULL)
        m_camera->applyTransform();
	else
        m_context->applyCamera();

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
	   logError( gluErrorString(error) );
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

const Camera* RenderManager::getCurrentCamera()
{
	return m_camera;
}

void RenderManager::setBackground(const Background &background)
{
    m_defaultBackground = background;
};

void RenderManager::applyBackground()
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
                m_defaultBackground.textures[0].bind();
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
                if(m_camera != NULL)
                    m_camera->applyOnlyRotation();
                else
                    m_context->applyCameraRotation();
                glDisable(GL_LIGHTING);
                glEnable(GL_TEXTURE_2D);
                glColor4f(1,1,1,1);

                // Render the front quad
                m_defaultBackground.textures[0].bind();
                glBegin(GL_QUADS);
                    glTexCoord2f(0, 0); glVertex3f(  10, -10, -10 );
                    glTexCoord2f(1, 0); glVertex3f( -10, -10, -10 );
                    glTexCoord2f(1, 1); glVertex3f( -10,  10, -10 );
                    glTexCoord2f(0, 1); glVertex3f(  10,  10, -10 );
                glEnd();

                // Render the left quad
                m_defaultBackground.textures[1].bind();
                glBegin(GL_QUADS);
                    glTexCoord2f(0, 0); glVertex3f(  10, -10,  10 );
                    glTexCoord2f(1, 0); glVertex3f(  10, -10, -10 );
                    glTexCoord2f(1, 1); glVertex3f(  10,  10, -10 );
                    glTexCoord2f(0, 1); glVertex3f(  10,  10,  10 );
                glEnd();

                // Render the back quad
                m_defaultBackground.textures[2].bind();
                glBegin(GL_QUADS);
                    glTexCoord2f(0, 0); glVertex3f( -10, -10,  10 );
                    glTexCoord2f(1, 0); glVertex3f(  10, -10,  10 );
                    glTexCoord2f(1, 1); glVertex3f(  10,  10,  10 );
                    glTexCoord2f(0, 1); glVertex3f( -10,  10,  10 );

                glEnd();

                // Render the right quad
                m_defaultBackground.textures[3].bind();
                glBegin(GL_QUADS);
                    glTexCoord2f(0, 0); glVertex3f( -10, -10, -10 );
                    glTexCoord2f(1, 0); glVertex3f( -10, -10,  10 );
                    glTexCoord2f(1, 1); glVertex3f( -10,  10,  10 );
                    glTexCoord2f(0, 1); glVertex3f( -10,  10, -10 );
                glEnd();

                // Render the top quad
                m_defaultBackground.textures[4].bind();
                glBegin(GL_QUADS);
                    glTexCoord2f(0, 1); glVertex3f( -10,  10, -10 );
                    glTexCoord2f(0, 0); glVertex3f( -10,  10,  10 );
                    glTexCoord2f(1, 0); glVertex3f(  10,  10,  10 );
                    glTexCoord2f(1, 1); glVertex3f(  10,  10, -10 );
                glEnd();

                // Render the bottom quad
                m_defaultBackground.textures[5].bind();
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
