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

#define POSTPROCESS_RESOLUTION 2048
#define OMNIDEPTH_RESOLUTION 1024

//#define SHOW_PASS_INFO

RenderManager::RenderManager() :
	m_context(NULL),
	m_camera(NULL),
	m_cameraChanged(true),
	m_drawDebug(false),
	m_inverse_modelview(NULL),
	m_modelview(NULL),
	m_inverse_projection(NULL),
	m_projection(NULL),
	m_screen_size(NULL),
	m_rendering(false),
	m_normalmap(NULL),
	m_diffusemap(NULL),
	m_specularmap(NULL),
	m_depthmap(NULL),
	m_bloommap(NULL),
	m_shadowmap(NULL),
	m_colormap(NULL),
	m_postprocessfbo(NULL)
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
	float aspect, scale;

	if(target.isOnScreen()) {
		int side = qMax(resizeTo.width(), resizeTo.height());
		glViewport((resizeTo.width() - side) / 2, (resizeTo.height() - side) / 2, side, side);
		aspect = 1;
		scale = 1;
		m_context->isResized();
	} else {
		int target_width = target.getWidth();
		int target_height = target.getHeight();
		glViewport(0, 0, target_width, target_height);
		if(target.isStretchedToScreen()) {
			int screen_width = resizeTo.width();
			int screen_height = resizeTo.height();
			//int side = qMax(screen_width, screen_height);
			aspect = float(screen_width * target_height) / float(screen_height * target_width);
			scale = aspect;
		} else {
			aspect = 1;
			scale = 1;
		}
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(viewpoint != NULL)
	{
		viewpoint->setProjection(aspect,scale,projection_nb);
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

void RenderManager::createResources()
{
	// Setting up shadow textures
	m_shadowmap = new RenderTexture2D("Shadowmap", POSTPROCESS_RESOLUTION, POSTPROCESS_RESOLUTION, GL_RGBA, GL_UNSIGNED_BYTE);
	new RenderTextureArray("Omni_Lightmap", OMNIDEPTH_RESOLUTION, OMNIDEPTH_RESOLUTION, 6, GL_DEPTH_COMPONENT, GL_FLOAT);

	// Setting up color map
	m_colormap = new RenderTexture2D("Colormap", POSTPROCESS_RESOLUTION, POSTPROCESS_RESOLUTION, GL_RGBA, GL_UNSIGNED_BYTE);

	// Setting up bloom map
	m_bloommap = new RenderTexture2D("Bloommap", POSTPROCESS_RESOLUTION, POSTPROCESS_RESOLUTION, GL_RGBA, GL_UNSIGNED_BYTE);

	// Setting up postprocessing FBO
	m_postprocessfbo = new FrameBufferObject(POSTPROCESS_RESOLUTION,POSTPROCESS_RESOLUTION, false, false);

	ShaderProgram shader = SHADER_PROGRAM_MANAGER.get("DEF_geometry_pass");
	if(shader.isValid() && shader->isUber()) {
		m_deferred_geometry = UberShader(*(static_cast<UberShaderData*>(*shader)));
	} else {
		if(!shader.isValid()) {
			logError("Could not find the DEF_geometry_pass ubershader for the deferred pipeline");
		} else if(!shader->isUber()) {
			logError("The DEF_geometry_pass shader is not an ubershader");
		}
	}
	shader = SHADER_PROGRAM_MANAGER.get("DEF_ambient_pass");
	if(shader.isValid() && shader->isUber()) {
		m_deferred_ambient = UberShader(*(static_cast<UberShaderData*>(*shader)));
	} else {
		if(!shader.isValid()) {
			logError("Could not find the DEF_ambient_pass ubershader for the deferred pipeline");
		} else if(!shader->isUber()) {
			logError("The DEF_ambient_pass shader is not an ubershader");
		}
	}
	shader = SHADER_PROGRAM_MANAGER.get("DEF_lighting_pass");
	if(shader.isValid() && shader->isUber()) {
		m_deferred_lighting = UberShader(*(static_cast<UberShaderData*>(*shader)));
	} else {
		if(!shader.isValid()) {
			logError("Could not find the DEF_lighting_pass ubershader for the deferred pipeline");
		} else if(!shader->isUber()) {
			logError("The DEF_lighting_pass shader is not an ubershader");
		}
	}

	// Deferred shading resources
	m_positionmap = new RenderTexture2D("DEF_Positionmap", POSTPROCESS_RESOLUTION, POSTPROCESS_RESOLUTION, GL_RGBA, GL_UNSIGNED_BYTE);
	m_normalmap = new RenderTexture2D("DEF_Normalmap", POSTPROCESS_RESOLUTION, POSTPROCESS_RESOLUTION, GL_RGBA, GL_UNSIGNED_BYTE);
	m_diffusemap = new RenderTexture2D("DEF_Diffusemap", POSTPROCESS_RESOLUTION, POSTPROCESS_RESOLUTION, GL_RGBA, GL_UNSIGNED_BYTE);
	m_specularmap = new RenderTexture2D("DEF_Specularmap", POSTPROCESS_RESOLUTION, POSTPROCESS_RESOLUTION, GL_RGBA, GL_UNSIGNED_BYTE);
	m_depthmap = new RenderTexture2D("DEF_Depthmap", POSTPROCESS_RESOLUTION, POSTPROCESS_RESOLUTION, GL_DEPTH_COMPONENT, GL_FLOAT);
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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// setting up engine uniforms hash
	m_inverse_modelview = new QMatrix4x4();
	m_engine_uniforms.insert(
		"inverse_modelview",
		new ShaderProgramData::Uniform<QMatrix4x4>("inverse_modelview",m_inverse_modelview, 1, 1)
	);
	m_modelview = new QMatrix4x4();
	m_engine_uniforms.insert(
		"modelview",
		new ShaderProgramData::Uniform<QMatrix4x4>("modelview",m_modelview, 1, 1)
	);
	m_inverse_projection = new QMatrix4x4();
	m_engine_uniforms.insert(
		"inverse_projection",
		new ShaderProgramData::Uniform<QMatrix4x4>("inverse_projection",m_inverse_projection, 1, 1)
	);
	m_projection = new QMatrix4x4();
	m_engine_uniforms.insert(
		"projection",
		new ShaderProgramData::Uniform<QMatrix4x4>("modelview",m_projection, 1, 1)
	);
	m_screen_size = new QVector2D();
	m_engine_uniforms.insert(
		"screen_size",
		new ShaderProgramData::Uniform<QVector2D>("screen_size",m_screen_size, 1, 1)
	);

	m_scene_ambient = new QVector3D(0.3,0.3,0.3);
	m_engine_uniforms.insert(
		"scene_ambient",
		new ShaderProgramData::Uniform<QVector3D>("scene_ambient",m_scene_ambient, 1, 1)
	);

	#ifndef SHOW_PASS_INFO
		Log::topicPolicy.insert("PASS_INFO", Log::POLICY_IGNORE);
	#endif
}

void RenderManager::render(double elapsed_time, SceneGraph* sg)
/* Deferred version */
{
	m_rendering = true;

	m_passinfo.context = m_context;
	m_passinfo.background_enabled = true;
	m_passinfo.setup_texture_matrices = false;
	m_passinfo.texturing_enabled = true;
	m_passinfo.type = FINAL_PASS;

	// Frame Begin
	for(QVector<IRenderable*>::iterator it = registeredManagees.begin();
		it != registeredManagees.end();
		it++) {
		(*it)->frameBegin(elapsed_time);
	}

	debug("PASS_INFO","begin");

	Viewpoint* viewpoint = m_camera;
	if(m_camera == NULL) {
		viewpoint = m_context->getViewpoint();
	}

	/// First pass - Render gbuffer
	QList< QPair<RenderTexture*, FrameBufferObject::AttachmentPoint> > mrts;
	mrts.push_back(QPair<RenderTexture*, FrameBufferObject::AttachmentPoint>(m_normalmap, FrameBufferObject::COLOR_ATTACHMENT_0));
	mrts.push_back(QPair<RenderTexture*, FrameBufferObject::AttachmentPoint>(m_diffusemap, FrameBufferObject::COLOR_ATTACHMENT_1));
	mrts.push_back(QPair<RenderTexture*, FrameBufferObject::AttachmentPoint>(m_specularmap, FrameBufferObject::COLOR_ATTACHMENT_2));
	mrts.push_back(QPair<RenderTexture*, FrameBufferObject::AttachmentPoint>(m_depthmap, FrameBufferObject::DEPTH_ATTACHMENT));
	RenderTarget srt(viewpoint, m_postprocessfbo, mrts , false, false);
	debug("PASS_INFO","geom pass");
	m_passinfo.ubershader_used = m_deferred_geometry;
	m_passinfo.type = FINAL_PASS;
	renderTarget(sg, srt);

	/// Second pass - lighting postprocess
	//// Modulate ambient
	m_passinfo.ubershader_used = m_deferred_ambient;
	QList<Texture> input_textures;
	input_textures.push_back(*m_diffusemap);
	postprocessPass(NULL,input_textures);

	//// For each light => light
	m_passinfo.ubershader_used = m_deferred_lighting;
	input_textures.push_front(*m_normalmap);
	input_textures.push_back(*m_specularmap);
	input_textures.push_back(*m_depthmap);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE, GL_ONE);
	for(int i = 0 ; i<LIGHTING_MANAGER.managees().count() ; i++) {
		// Render depthmap
		Light* light = LIGHTING_MANAGER.managees().at(i);
		light->sendParameters(0);
		postprocessPass(NULL,input_textures);
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

/*
	debugDisplayTexture(*m_normalmap,0,0,256,256);
	debugDisplayTexture(*m_diffusemap,256,0,256,256);
	debugDisplayTexture(*m_specularmap,512,0,256,256);
	debugDisplayTexture(*m_depthmap,768,0,256,256);
*/

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

	m_rendering = false;
}
/* Normal version
{
	RenderPassInfo pass_info;
	pass_info.setup_texture_matrices = false;

	// Frame Begin
	for(QVector<IRenderable*>::iterator it = registeredManagees.begin();
		it != registeredManagees.end();
		it++) {
		(*it)->frameBegin(elapsed_time);
	}

	debug("PASS_INFO","begin");

	//clear shadowmap
	Material clear_material = MATERIAL_MANAGER.get("clear_target");
	postprocessPass(m_shadowmap, clear_material);
	/// Render shadowmap
	for(int i = 0 ; i<LIGHTING_MANAGER.managees().count() ; i++) {
		// Render depthmap
		Light* light = LIGHTING_MANAGER.managees().at(i);

		Material cast_shadow_material = MATERIAL_MANAGER.get("shadow_cast");
		Material receive_shadow_material = MATERIAL_MANAGER.get("shadow_receive");

		Texture depthtex = TEXTURE_MANAGER.get("Omni_Lightmap");

		if(depthtex.isValid()) {
			RenderTexture* rt;
			rt = static_cast<RenderTexture*>(*depthtex);
			FrameBufferObject fbo(rt->getWidth(), rt->getHeight(), false, false);
			RenderTarget srt(light, &fbo, rt, FrameBufferObject::DEPTH_ATTACHMENT, false, false);
			debug("PASS_INFO","cast light " + QString().setNum(i));
			glCullFace(GL_FRONT);
			pass_info.setup_texture_matrices = true;
			pass_info.forced_material = cast_shadow_material;
			pass_info.type = CAST_SHADOW_PASS;
			renderTarget(sg, srt, pass_info);
			pass_info.setup_texture_matrices = false;
			glCullFace(GL_BACK);
		}

		{
			FrameBufferObject fbo(m_shadowmap->getWidth(), m_shadowmap->getHeight(), false, true);
			Viewpoint* viewpoint = m_camera;
			if(m_camera == NULL) {
				viewpoint = m_context->getViewpoint();
			}
			RenderTarget srt(viewpoint, &fbo, m_shadowmap, FrameBufferObject::COLOR_ATTACHMENT_0, false, true);
			debug("PASS_INFO","receive light " + QString().setNum(i));
			pass_info.forced_material = receive_shadow_material;
			pass_info.type = RECEIVE_SHADOW_PASS;
			renderTarget(sg, srt, pass_info);
		}
	}

	//blur shadowmap
	Material blur_material = MATERIAL_MANAGER.get("blur_shadowmap");
	postprocessPass(m_shadowmap, blur_material);
	postprocessPass(m_shadowmap, blur_material);

	// Render
	for(int i=0 ; i<m_rts.length() ; i++) {
		RenderTarget* rt = m_rts[i];
		if(rt != NULL) {
			debug("PASS_INFO","rtt " + QString().setNum(i));
			pass_info.forced_material = Material();
			pass_info.type = FINAL_PASS;
			renderTarget(sg, *rt, pass_info);
		}
	}
	Viewpoint* viewpoint = m_camera;
	if(m_camera == NULL) {
		viewpoint = m_context->getViewpoint();
	}

	{
		FrameBufferObject fbo(m_shadowmap->getWidth(), m_shadowmap->getHeight(), false, true);
		Viewpoint* viewpoint = m_camera;
		if(m_camera == NULL) {
			viewpoint = m_context->getViewpoint();
		}
		QList< QPair<RenderTexture*, FrameBufferObject::AttachmentPoint> > mrts;
		mrts.push_back(QPair<RenderTexture*, FrameBufferObject::AttachmentPoint>(m_colormap, FrameBufferObject::COLOR_ATTACHMENT_0));
		mrts.push_back(QPair<RenderTexture*, FrameBufferObject::AttachmentPoint>(m_bloommap, FrameBufferObject::COLOR_ATTACHMENT_1));
		RenderTarget srt(viewpoint, &fbo, mrts, false, true);
		debug("PASS_INFO","mrt color bloom");
		pass_info.forced_material = Material();
		pass_info.type = FINAL_PASS;
		renderTarget(sg, srt, pass_info);
	}

	//blur bloommap
	blur_material = MATERIAL_MANAGER.get("blur");
	postprocessPass(m_bloommap, blur_material);
	postprocessPass(m_bloommap, blur_material);

	Material compose_bloom = MATERIAL_MANAGER.get("compose_bloom");
	postprocessPass(NULL, compose_bloom);

	debugDisplayTexture(*m_bloommap,0,0,256,256);

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
*/

void RenderManager::renderTarget(SceneGraph* sg, RenderTarget& target)
{
	Viewpoint* viewpoint = target.getViewpoint();

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

		if(m_passinfo.setup_texture_matrices) {
			target.setTextureMatrix(pass_nb);
			debugGL("setting up texture matrices");
		}

		// set inverse camera transform
		// Transpose because Qt does not have the same major than OpenGL
		double camera_transform[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, camera_transform);
		*m_modelview = QMatrix4x4(camera_transform).transposed();
		Matrix4d mat(camera_transform);
		mat.invertAndTranspose();
		*m_inverse_modelview = QMatrix4x4(mat.values);
		glGetDoublev(GL_PROJECTION_MATRIX, camera_transform);
		*m_projection = QMatrix4x4(camera_transform).transposed();
		mat = Matrix4d(camera_transform);
		mat.invertAndTranspose();
		*m_inverse_projection = QMatrix4x4(mat.values);
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

		for(QVector<IRenderable*>::iterator it = registeredManagees.begin();
			it != registeredManagees.end();
			it++) {

			if(m_passinfo.type == CAST_SHADOW_PASS && !((*it)->castsShadows()))
				continue;
			if(m_passinfo.type == RECEIVE_SHADOW_PASS && !((*it)->receivesShadows()))
				continue;

			glPushMatrix();
			if((*it)->isTransparent()){
				transparent_renderables.push_back(*it); // Transparent renderables are deferred for later rendering
			} else {
				(*it)->render();
			}
			glPopMatrix();
		}

		for(QList<IRenderable*>::iterator it = transparent_renderables.begin();
			it != transparent_renderables.end();
			it++) {
			glPushMatrix();
			(*it)->render();
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
			if(target.isOnScreen()) {
				sg->getManipulator()->draw(m_context);
			}
		#endif

		target.passDone();
	}

	target.release();
}

void RenderManager::postprocessPass(RenderTexture* target_texture, QList<Texture> input_textures)
{
	debugGL("before postprocessing");

	if(target_texture != NULL) {
		m_postprocessfbo->bind();
		m_postprocessfbo->attachTexture(target_texture, FrameBufferObject::COLOR_ATTACHMENT_0, GL_TEXTURE_2D);
		m_postprocessfbo->commitTextures(0);
	}

	debugGL("preparing FBO for postprocessing");
	glClear(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	m_passinfo.ubershader_used->use();
	m_passinfo.ubershader_used->setAllUniforms();

	int width;
	int height;

	if(target_texture != NULL) {
		width = m_postprocessfbo->getWidth();
		height = m_postprocessfbo->getHeight();
	} else {
		QRect geom = m_context->geometry();
		width = geom.width();
		height = geom.height();
	}

	for(int i=0 ; i< input_textures.size() ; i++) {
		input_textures[i]->bind(i);
	}

	glViewport(0,0,width,height);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f);
		glVertex3d(-1,-1,0.0f);
		glTexCoord2f(1.0f,0.0f);
		glVertex3d(1,-1,0.0f);
		glTexCoord2f(1.0f,1.0f);
		glVertex3d(1,1,0.0f);
		glTexCoord2f(0.0f,1.0f);
		glVertex3d(-1,1,0.0f);
	glEnd();

	for(int i=0 ; i< input_textures.size() ; i++) {
		input_textures[i]->release(i);
	}

	m_passinfo.ubershader_used->unset();

	if(target_texture != NULL) {
		m_postprocessfbo->swapTextures();
		m_postprocessfbo->release();
	}
}

void RenderManager::debugDisplayTexture(Texture texture, int x, int y, int width, int height)
{
	glClear(GL_DEPTH_BUFFER_BIT);

	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glViewport(x,y,width,height);
	texture->bind();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f);
		glVertex3d(-1,-1,0.0f);
		glTexCoord2f(1.0f,0.0f);
		glVertex3d(1,-1,0.0f);
		glTexCoord2f(1.0f,1.0f);
		glVertex3d(1,1,0.0f);
		glTexCoord2f(0.0f,1.0f);
		glVertex3d(-1,1,0.0f);
	glEnd();

	glEnable(GL_LIGHTING);
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
}

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

RenderManager::RenderPassInfo* RenderManager::getRenderPassInfo()
{
	RenderPassInfo* ret = NULL;

	if(m_rendering) {
		ret = &m_passinfo;
	} else {
		logError("Asked for RenderPassInfo outside the rendering loop");
	}

	return ret;
}
