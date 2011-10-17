#include "core/managers/rendermanager.h"
#include "core/framework/glwidget.h"
#include "core/scenegraph/scenegraph.h"
#include "core/properties/irenderable.h"
#include "core/properties/camera.h"
#include "core/managers/lightingmanager.h"
#include "core/managers/physicsmanager.h"
#include "core/properties/light.h"
#include <core/graphics/rendertarget.h>
#include <core/graphics/rt2d.h>
#include <core/graphics/rtcubemap.h>
#include <core/graphics/rtarray.h>

#include <core/resources/managers.h>

#include <QtOpenGL>
#include <QRect>
#include <math.h>

#include "core/log/log.h"

#ifdef WITH_TOOLS
	#include "tools/widgets/renderwidget.h"
	#include "tools/3D/manipulator.h"
#endif

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif


//#define SHOW_PASS_INFO

RenderManager::RenderManager() : m_context(NULL), m_camera(NULL), m_cameraChanged(true), m_drawDebug(false)
{
	m_defaultBackground.type = NO_CLEAR;
}

RenderManager::~RenderManager()
{
}

void RenderManager::setupProjection(RenderTarget& target, int projection_nb)
{
	Viewpoint* viewpoint = target.getViewpoint();

	if(viewpoint == NULL) {
		logError("No viewpoint : can't set projection");
		return;
	}

	QSize resizeTo;
	bool needResize = m_context->needResize(&resizeTo);
	float aspect;

	if(target.isOnScreen()) {
		int side = qMax(resizeTo.width(), resizeTo.height());
		glViewport((resizeTo.width() - side) / 2, (resizeTo.height() - side) / 2, side, side);
		aspect = 1;
		m_context->isResized();
	} else {
		int target_width = target.getWidth();
		int target_height = target.getHeight();
		glViewport(0, 0, target_width, target_height);
		if(target.isStretchedToScreen()) {
			int screen_width = resizeTo.width();
			int screen_height = resizeTo.height();
			//int side = qMax(screen_width, screen_height);
			aspect = (screen_width * target_height) / (screen_height * target_width);
		} else {
			aspect = 1;
		}
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(viewpoint != NULL)
	{
		viewpoint->setProjection(aspect,projection_nb);
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

	// setting up engine uniforms hash
	m_inverse_transpose_camera = new QMatrix4x4();
	m_engine_uniforms.insert(
		"inverse_transpose_camera",
		new ShaderProgramData::Uniform<QMatrix4x4>("inverse_transpose_camera",m_inverse_transpose_camera, 1, 1)
	);
	m_screen_size = new QVector2D();
	m_engine_uniforms.insert(
		"screen_size",
		new ShaderProgramData::Uniform<QVector2D>("screen_size",m_screen_size, 1, 1)
	);

	// Setting up shadow textures
	/*
	new RenderTexture2D("Shadowmap", 512, 512, GL_RGBA, GL_UNSIGNED_BYTE);
	new RenderTextureArray("Omni_Lightmap", 256, 256, 6, GL_DEPTH_COMPONENT, GL_FLOAT);
*/
	m_shadowmap = static_cast<RenderTexture*>(*TEXTURE_MANAGER.get("Shadowmap"));

	#ifndef SHOW_PASS_INFO
		Log::topicPolicy.insert("PASS_INFO", Log::POLICY_IGNORE);
	#endif
}

void RenderManager::render(double elapsed_time, SceneGraph* sg)
{
	// Frame Begin
	for(QVector<IRenderable*>::iterator it = registeredManagees.begin();
		it != registeredManagees.end();
		it++) {
		(*it)->frameBegin(elapsed_time);
	}

	debug("PASS_INFO","begin");

	//clear shadowmap
	m_shadowmap->clear();
	/// Render shadowmap
	for(int i = 0 ; i<LIGHTING_MANAGER.managees().count() ; i++) {
		// Render depthmap
		Light* light = LIGHTING_MANAGER.managees().at(i);

		Material cast_shadow_material = MATERIAL_MANAGER.get("shadow_cast");
		Material receive_shadow_material = MATERIAL_MANAGER.get("shadow_receive");

		Texture depthtex = TEXTURE_MANAGER.get("Omni_Lightmap");
		if(depthtex.isValid()) {
			RenderTexture* rt;
			FrameBufferObject fbo(256, 256, false, false);
			rt = static_cast<RenderTexture*>(*depthtex);
			RenderTarget srt(light, &fbo, rt, FrameBufferObject::DEPTH_ATTACHMENT, false, false);
			debug("PASS_INFO","cast light " + QString().setNum(i));
			glCullFace(GL_FRONT);
			renderTarget(sg, srt, cast_shadow_material, true);
			glCullFace(GL_BACK);
		}

		{
			FrameBufferObject fbo(512, 512, false, true);
			Viewpoint* viewpoint = m_camera;
			if(m_camera == NULL) {
				viewpoint = m_context->getViewpoint();
			}
			RenderTarget srt(viewpoint, &fbo, m_shadowmap, FrameBufferObject::COLOR_ATTACHMENT, false, true);
			debug("PASS_INFO","receive light " + QString().setNum(i));
			renderTarget(sg, srt, receive_shadow_material);
		}
	}

	// Render
	for(int i=0 ; i<m_rts.length() ; i++) {
		RenderTarget* rt = m_rts[i];
		if(rt != NULL) {
			debug("PASS_INFO","rtt " + QString().setNum(i));
			renderTarget(sg, *rt);
		}
	}

	Viewpoint* viewpoint = m_camera;
	if(m_camera == NULL) {
		viewpoint = m_context->getViewpoint();
	}
	RenderTarget crt(viewpoint);
	debug("PASS_INFO","screen");
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

void RenderManager::renderTarget(SceneGraph* sg, RenderTarget& target, bool setup_texture_matrices)
{
	renderTarget(sg,target, Material() , setup_texture_matrices);
}

void RenderManager::renderTarget(SceneGraph* sg, RenderTarget& target, Material forced_material, bool setup_texture_matrices)
{
	Viewpoint* viewpoint = target.getViewpoint();
	bool material_is_overridden = forced_material.isValid();

	if(viewpoint == NULL) {
		logError("No viewpoint to render from, you must set a camera");
		return;
	}

	target.bind();

	for(int pass_nb=0 ; pass_nb<target.getNbPass() ; pass_nb++) {

		target.setupPass(pass_nb);

		QList<IRenderable*> transparent_renderables;

		if(m_context == NULL) {
			return;
		}

		if(m_defaultBackground.type == SKYBOX) {
			setupProjection(target,pass_nb);
			glLoadIdentity();
			applyBackground(target,pass_nb);
		} else {
			glLoadIdentity();
			applyBackground(target,pass_nb);
			setupProjection(target,pass_nb);
			glLoadIdentity();
		}

		viewpoint->applyTransform(pass_nb);

		debugGL("initiating cameras");

		if(setup_texture_matrices) {
			target.setTextureMatrix(pass_nb);
			debugGL("setting up texture matrices");
		}

		// set inverse camera transform
		double camera_transform[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, camera_transform);
		Matrix4d mat(camera_transform);
		mat.invertAndTranspose();
		*m_inverse_transpose_camera = QMatrix4x4(mat.values);
		// set screensize
		if(target.isOnScreen()) {
			QRect geom = m_context->geometry();
			m_screen_size->setX(geom.width());
			m_screen_size->setY(geom.height());
		} else {
			m_screen_size->setX(target.getWidth());
			m_screen_size->setY(target.getHeight());
		}

		glEnable(GL_LIGHTING);
		for(int index = 0; index < LIGHTING_MANAGER.managees().count(); index++) {
			LIGHTING_MANAGER.managees().at(index)->sendParameters(index);
		}

		if(material_is_overridden) {
			forced_material->apply(0);
		}

		for(QVector<IRenderable*>::iterator it = registeredManagees.begin();
			it != registeredManagees.end();
			it++) {
			glPushMatrix();
			if((*it)->isTransparent()){
				transparent_renderables.push_back(*it); // Transparent renderables are deferred for later rendering
			} else {
				(*it)->render(m_context, material_is_overridden);
			}
			glPopMatrix();
		}

		for(QList<IRenderable*>::iterator it = transparent_renderables.begin();
			it != transparent_renderables.end();
			it++) {
			glPushMatrix();
			(*it)->render(m_context, material_is_overridden);
			glPopMatrix();
		}

		if(material_is_overridden) {
			forced_material->unset(0);
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
			if(target.isOnScreen()) {
				sg->getManipulator()->draw(m_context);
			}
		#endif

		target.passDone();
	}

	target.release();
}

void RenderManager::postprocessPass(RenderTarget& target, QGLShaderProgram* program)
{
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

void RenderManager::applyBackground(RenderTarget& target, int projection_nb)
{
	Viewpoint* viewpoint = target.getViewpoint();

	if(viewpoint == NULL) {
		logError("No viewpoint : can't apply background");
		return;
	}

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
				viewpoint->applyOnlyRotation(projection_nb);
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

const QHash<QString, ShaderProgramData::UniformBase*>& RenderManager::getEngineUniforms()
{
	return m_engine_uniforms;
}

