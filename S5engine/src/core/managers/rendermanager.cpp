#include "core/managers/rendermanager.h"
#include "core/abstraction/opengl2/gl2renderdevice.h"
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
#include <GL/glu.h>
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

#define MAX_LIGHTS 8

//#define SHOW_PASS_INFO

RenderManager::RenderManager() :
	m_context(0),
	m_device(0),
	m_camera(0),
	m_cameraChanged(true),
	m_drawDebug(false),
	m_inverse_modelview(0),
	m_modelview(0),
	m_texture_matrices(0),
	m_inverse_projection(0),
	m_projection(0),
	m_screen_size(0),
	m_scene_ambient(0),
	m_sky_color(0),
	m_sun_pos(0),
	m_rendering(false),
	m_normalmap(0),
	m_diffusemap(0),
	m_specularmap(0),
	m_depthmap(0),
	m_bloommap(0),
	m_shadowmap(0),
	m_colormap(0),
	m_lightscatteringmap_high(0),
	m_lightscatteringmap_low(0),
	m_postprocessfbo(0),
	m_lowres_postprocessfbo(0)
{
	m_defaultBackground.type = Background::NO_CLEAR;

	m_device = new IRD::GL2RenderDevice();

	#ifdef WITH_TOOLS
		m_widget = 0;
	#endif
}

RenderManager::~RenderManager()
{
}

void RenderManager::setupProjection(RenderTarget& target, int projection_nb)
{
	Viewpoint* viewpoint = target.getViewpoint();

	if(viewpoint == 0) {
		logError("No viewpoint : can't set projection");
		return;
	}

	float aspect, scale;

	if(target.isOnScreen() || target.isStretchedToScreen()) {
/*
		int min_side = qMin(m_viewport_size.x, m_viewport_size.y);
		int max_side = qMax(m_viewport_size.x, m_viewport_size.y);
		glViewport((m_viewport_size.y - max_side) / 2, (m_viewport_size.x - max_side) / 2, max_side, max_side);
		aspect = 1;
		scale = 1; //float(min_side)/float(max_side);
*/
		glViewport(0, 0, m_viewport_size.y, m_viewport_size.x);
		aspect = float(m_viewport_size.y)/float(m_viewport_size.x);
		scale = 1;
	} else {
		int target_width = target.getWidth();
		int target_height = target.getHeight();
		glViewport(0, 0, target_width, target_height);
		aspect = 1;
		scale = 1;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(viewpoint != 0)
	{
		viewpoint->setProjection(aspect,scale,projection_nb);
	}

	glMatrixMode(GL_MODELVIEW);
}

void RenderManager::createResources()
{
	QSize size = m_context->size();
	QSize halfsize = size;
	halfsize.setHeight(size.height()/2);
	halfsize.setWidth(size.width()/2);

	// Setting up shadow textures
	m_shadowmap = new RenderTexture2D("DEF_Shadowmap", size.height(), size.width(), IRD::Texture::TF_RGBA8);

	// Setting up color map
	m_colormap = new RenderTexture2D("Colormap", size.height(), size.width(), IRD::Texture::TF_RGBA8);

	// Setting up bloom map
	m_bloommap = new RenderTexture2D("Bloommap", halfsize.height(), halfsize.width(), IRD::Texture::TF_RGBA8);

	// Setting up lightscattering map
	m_lightscatteringmap_low = new RenderTexture2D("LightScatteringL", size.height(), size.width(), IRD::Texture::TF_RGBA8);
	m_lightscatteringmap_high = new RenderTexture2D("LightScatteringH", size.height(), size.width(), IRD::Texture::TF_RGBA8);

	// Setting up postprocessing FBO
	m_postprocessfbo = new FrameBufferObject(size.height(), size.width(), false, false);
	m_lowres_postprocessfbo = new FrameBufferObject(halfsize.height(), halfsize.width(), false, false);

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
	shader = SHADER_PROGRAM_MANAGER.get("DEF_shadowmap_pass");
	if(shader.isValid() && shader->isUber()) {
		m_generate_shadowmap = UberShader(*(static_cast<UberShaderData*>(*shader)));
	} else {
		if(!shader.isValid()) {
			logError("Could not find the DEF_shadowmap_pass ubershader for the deferred pipeline");
		} else if(!shader->isUber()) {
			logError("The DEF_shadowmap_pass shader is not an ubershader");
		}
	}
	shader = SHADER_PROGRAM_MANAGER.get("DEF_vertical_blur_pass");
	if(shader.isValid() && shader->isUber()) {
		m_vertical_blur = UberShader(*(static_cast<UberShaderData*>(*shader)));
	} else {
		if(!shader.isValid()) {
			logError("Could not find the DEF_vertical_blur_pass ubershader for the deferred pipeline");
		} else if(!shader->isUber()) {
			logError("The DEF_vertical_blur_pass shader is not an ubershader");
		}
	}
	shader = SHADER_PROGRAM_MANAGER.get("DEF_horizontal_blur_pass");
	if(shader.isValid() && shader->isUber()) {
		m_horizontal_blur = UberShader(*(static_cast<UberShaderData*>(*shader)));
	} else {
		if(!shader.isValid()) {
			logError("Could not find the DEF_horizontal_blur_pass ubershader for the deferred pipeline");
		} else if(!shader->isUber()) {
			logError("The DEF_horizontal_blur_pass shader is not an ubershader");
		}
	}
	shader = SHADER_PROGRAM_MANAGER.get("DEF_bloom_pass");
	if(shader.isValid() && shader->isUber()) {
		m_bloom = UberShader(*(static_cast<UberShaderData*>(*shader)));
	} else {
		if(!shader.isValid()) {
			logError("Could not find the DEF_bloom_pass ubershader for the deferred pipeline");
		} else if(!shader->isUber()) {
			logError("The DEF_bloom_pass shader is not an ubershader");
		}
	}
	shader = SHADER_PROGRAM_MANAGER.get("light_scattering");
	if(shader.isValid() && shader->isUber()) {
		m_lightscattering = UberShader(*(static_cast<UberShaderData*>(*shader)));
	} else {
		if(!shader.isValid()) {
			logError("Could not find the light_scattering ubershader");
		} else if(!shader->isUber()) {
			logError("The light_scattering shader is not an ubershader");
		}
	}
	shader = SHADER_PROGRAM_MANAGER.get("depth_only");
	if(shader.isValid() && shader->isUber()) {
		m_depth = UberShader(*(static_cast<UberShaderData*>(*shader)));
	} else {
		if(!shader.isValid()) {
			logError("Could not find the depth_only ubershader");
		} else if(!shader->isUber()) {
			logError("The depth_only shader is not an ubershader");
		}
	}
	shader = SHADER_PROGRAM_MANAGER.get("forward");
	if(shader.isValid() && shader->isUber()) {
		m_forward = UberShader(*(static_cast<UberShaderData*>(*shader)));
	} else {
		if(!shader.isValid()) {
			logError("Could not find the forward ubershader");
		} else if(!shader->isUber()) {
			logError("The forward shader is not an ubershader");
		}
	}

	// Deferred shading resources
	m_positionmap = new RenderTexture2D("DEF_Positionmap", size.height(), size.width(), IRD::Texture::TF_RGBA8);
	m_normalmap = new RenderTexture2D("DEF_Normalmap", size.height(), size.width(), IRD::Texture::TF_RGBA8);
	m_diffusemap = new RenderTexture2D("DEF_Diffusemap", size.height(), size.width(), IRD::Texture::TF_RGBA8);
	m_specularmap = new RenderTexture2D("DEF_Specularmap", size.height(), size.width(), IRD::Texture::TF_RGBA8);
	//m_depthmap = new RenderTexture2D("DEF_Depthmap", size.height(), size.width(), IRD::Texture::TF_DEPTH);
	m_depthmap = new RenderTexture2D("DEF_Depthmap", size.height(), size.width(), IRD::Texture::TF_DEPTH_STENCIL);

	// SSS resources
	m_sssbuffer[0] = new RenderTexture2D("SSS1", 512, 512, IRD::Texture::TF_RGBA8);
	m_sssbuffer[1] = new RenderTexture2D("SSS2", 512, 512, IRD::Texture::TF_RGBA8);
	m_sssbuffer[2] = new RenderTexture2D("SSS3", 512, 512, IRD::Texture::TF_RGBA8);
	m_sss_fbo = new FrameBufferObject(512, 512, false, false);
}

void RenderManager::testViewportResize()
{
	QSize resizeTo;
	bool needResize = m_context->needResize(&resizeTo);

	if(needResize)
	{
		updateResources(resizeTo.height(), resizeTo.width());
		m_context->isResized();
	}

	m_viewport_size.x = resizeTo.height();
	m_viewport_size.y = resizeTo.width();
}

void RenderManager::updateResources(int new_height, int new_width)
{
	m_postprocessfbo->resize(new_height, new_width);
	m_lowres_postprocessfbo->resize(new_height/2, new_width/2);
	m_shadowmap->resize(new_height, new_width);
	m_colormap->resize(new_height, new_width);
	m_bloommap->resize(new_height/2, new_width/2);
	m_positionmap->resize(new_height, new_width);
	m_normalmap->resize(new_height, new_width);
	m_diffusemap->resize(new_height, new_width);
	m_specularmap->resize(new_height, new_width);
	m_depthmap->resize(new_height, new_width);
	m_lightscatteringmap_high->resize(new_height, new_width);
	m_lightscatteringmap_low->resize(new_height/2, new_width/2);
}

void RenderManager::init(GLWidget* context)
{
	m_context = context;

	CAMERA_MANAGER.setDebugCamera(context->getViewpoint());

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
	if(!m_inverse_modelview) m_inverse_modelview = new QMatrix4x4();
	m_engine_uniforms.insert(
		"inverse_modelview",
		new ShaderProgramData::Uniform<QMatrix4x4>("inverse_modelview",m_inverse_modelview, 1, 1)
	);

	if(!m_modelview) m_modelview = new QMatrix4x4();
	m_engine_uniforms.insert(
		"modelview",
		new ShaderProgramData::Uniform<QMatrix4x4>("modelview",m_modelview, 1, 1)
	);

	if(!m_inverse_projection) m_inverse_projection = new QMatrix4x4();
	m_engine_uniforms.insert(
		"inverse_projection",
		new ShaderProgramData::Uniform<QMatrix4x4>("inverse_projection",m_inverse_projection, 1, 1)
	);

	if(!m_projection) m_projection = new QMatrix4x4();
	m_engine_uniforms.insert(
		"projection",
		new ShaderProgramData::Uniform<QMatrix4x4>("projection",m_projection, 1, 1)
	);

	if(!m_texture_matrices) m_texture_matrices = new QMatrix4x4[6];
	for(int i=0 ; i<6; i++) {
		QString name = "texture_matrix"+QString().setNum(i);
		m_engine_uniforms.insert(
			name,
			new ShaderProgramData::Uniform<QMatrix4x4>(name,&(m_texture_matrices[i]), 1, 1)
		);
	}

	if(!m_screen_size) m_screen_size = new QVector2D();
	m_engine_uniforms.insert(
		"screen_size",
		new ShaderProgramData::Uniform<QVector2D>("screen_size",m_screen_size, 1, 1)
	);

	if(!m_sky_color)	m_sky_color = new QVector3D(0.5,0.5,0.5);
	m_engine_uniforms.insert(
		"sky_color",
		new ShaderProgramData::Uniform<QVector3D>("sky_color",m_sky_color, 1, 1)
	);

	if(!m_sun_pos)	m_sun_pos = new QVector2D(0.5,0.5);
	m_engine_uniforms.insert(
		"sun_pos",
		new ShaderProgramData::Uniform<QVector2D>("sun_pos",m_sun_pos, 1, 1)
	);

	if(!m_scene_ambient)	m_scene_ambient = new QVector3D(0.1,0.1,0.1);
	m_engine_uniforms.insert(
		"scene_ambient",
		new ShaderProgramData::Uniform<QVector3D>("scene_ambient",m_scene_ambient, 1, 1)
	);

	#ifndef SHOW_PASS_INFO
		Log::topicPolicy.insert("PASS_INFO", Log::POLICY_IGNORE);
	#endif
}

void RenderManager::renderDeferred(SceneGraph* sg, Viewpoint* viewpoint)
{
	/// Cleans
	clearTexture(m_bloommap);
	clearTexture(m_colormap);
	if(m_options.m_lightscattering_enabled)
	{
		clearTexture(m_lightscatteringmap_low);
	}

	/// First pass - Render gbuffer
	QList< QPair<RenderTexture*, IRD::FrameBuffer::Attachment> > mrts;
	mrts.push_back(QPair<RenderTexture*, IRD::FrameBuffer::Attachment>(m_normalmap, IRD::FrameBuffer::COLOR_ATTACHMENT_0));
	mrts.push_back(QPair<RenderTexture*, IRD::FrameBuffer::Attachment>(m_diffusemap, IRD::FrameBuffer::COLOR_ATTACHMENT_1));
	mrts.push_back(QPair<RenderTexture*, IRD::FrameBuffer::Attachment>(m_specularmap, IRD::FrameBuffer::COLOR_ATTACHMENT_2));
	if(m_options.m_lightscattering_enabled)
	{
		mrts.push_back(QPair<RenderTexture*, IRD::FrameBuffer::Attachment>(m_lightscatteringmap_high, IRD::FrameBuffer::COLOR_ATTACHMENT_3));
	}
	mrts.push_back(QPair<RenderTexture*, IRD::FrameBuffer::Attachment>(m_depthmap, IRD::FrameBuffer::DEPTH_STENCIL_ATTACHMENT));
	//mrts.push_back(QPair<RenderTexture*, IRD::FrameBuffer::Attachment>(m_depthmap, IRD::FrameBuffer::STENCIL_ATTACHMENT));
	RenderTarget srt(viewpoint, m_postprocessfbo, mrts , false, true);
	debug("PASS_INFO","geom pass");
	m_passinfo.ubershader_used = m_deferred_geometry;
	m_passinfo.ubershader_used->resetParams();
	m_passinfo.ubershader_used->setParamValue(UberShaderDefine::Type(UberShaderDefine::LIGHT_SCATTERING),m_options.m_lightscattering_enabled);
	m_passinfo.lighting_enabled = false;
	m_passinfo.type = RenderPassInfo::DEF_GEOMETRY_PASS;
	glBlendFunc(GL_ONE, GL_ZERO);
	renderTarget(sg, srt);
	m_postprocessfbo->clearAttachments();

	/// Second pass - lighting postprocess
	//// Modulate ambient
	m_passinfo.ubershader_used = m_deferred_ambient;
	m_passinfo.ubershader_used->resetParams();
	m_passinfo.type = RenderPassInfo::DEF_LIGHTING_PASS;
	m_postprocessfbo->bind();
	m_postprocessfbo->attachTexture(m_colormap, IRD::FrameBuffer::COLOR_ATTACHMENT_0);
	m_postprocessfbo->commitTextures(0);

	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	m_passinfo.ubershader_used->use();
	m_passinfo.ubershader_used->setAllUniforms();

	int width;
	int height;

	width = m_postprocessfbo->getWidth();
	height = m_postprocessfbo->getHeight();
	m_diffusemap->bind(0);
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
	m_diffusemap->release(0);
	m_passinfo.ubershader_used->unset();

/*
	QList<Texture> input_textures;
	input_textures.push_back(*m_diffusemap);
	postprocessPass(0,input_textures);
*/
	//// For each light => light
	m_passinfo.ubershader_used = m_deferred_lighting;
	m_passinfo.ubershader_used->resetParams();
	m_passinfo.ubershader_used->setParamValue(UberShaderDefine::BLOOM, m_options.m_bloom_enabled);
/*
	input_textures.push_front(*m_normalmap);
	input_textures.push_back(*m_specularmap);
	input_textures.push_back(*m_depthmap);
*/
	m_normalmap->bind(0);
	m_diffusemap->bind(1);
	m_specularmap->bind(2);
	m_depthmap->bind(3);

/*
	mrts.clear();
	mrts.push_back(QPair<RenderTexture*, IRD::FrameBuffer::Attachment>(m_colormap, IRD::FrameBuffer::COLOR_ATTACHMENT_0));
	mrts.push_back(QPair<RenderTexture*, IRD::FrameBuffer::Attachment>(m_bloommap, IRD::FrameBuffer::COLOR_ATTACHMENT_1));
*/
	glBlendFunc(GL_ONE, GL_ONE);
	for(int i = 0 ; i<LIGHTING_MANAGER.managees().count() ; i++) {
		// Render depthmap
		Light* light = LIGHTING_MANAGER.managees().at(i);
		light->sendParameters(0);
		//postprocessPass(mrts,input_textures);
		//postprocessPass(0,input_textures);

		m_passinfo.ubershader_used->setParamValue(UberShaderDefine::Type(UberShaderDefine::LIGHT_OMNI),light->getType() == Light::OMNI);
		m_passinfo.ubershader_used->setParamValue(UberShaderDefine::Type(UberShaderDefine::LIGHT_SPOT),light->getType() == Light::SPOT);
		m_passinfo.ubershader_used->setParamValue(UberShaderDefine::Type(UberShaderDefine::LIGHT_SUN),light->getType() == Light::SUN);

		bool shadow = light->castsShadows() && m_options.m_shadows_enabled;

		m_passinfo.ubershader_used->setParamValue(UberShaderDefine::SHADOWED, shadow);
		m_passinfo.ubershader_used->use();
		m_passinfo.ubershader_used->setAllUniforms();

		if(shadow)
		{
			m_postprocessfbo->clearAttachments();
			m_postprocessfbo->attachTexture(m_shadowmap, IRD::FrameBuffer::COLOR_ATTACHMENT_0);
			m_postprocessfbo->commitTextures(0);
			m_passinfo.ubershader_used->unset();
			m_passinfo.ubershader_used = m_generate_shadowmap;
			m_passinfo.ubershader_used->setParamValue(UberShaderDefine::Type(UberShaderDefine::LIGHT_OMNI),light->getType() == Light::OMNI);
			m_passinfo.ubershader_used->setParamValue(UberShaderDefine::Type(UberShaderDefine::LIGHT_SPOT),light->getType() == Light::SPOT);
			m_passinfo.ubershader_used->setParamValue(UberShaderDefine::Type(UberShaderDefine::LIGHT_SUN),light->getType() == Light::SUN);
			m_passinfo.ubershader_used->use();

			m_depthmap->bind(0);
			light->getRenderTexture()->bind(1);

			m_passinfo.ubershader_used->setAllUniforms();

			glClear(GL_COLOR_BUFFER_BIT);
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

		}

		m_passinfo.ubershader_used->unset();
		m_passinfo.ubershader_used = m_deferred_lighting;
		m_passinfo.ubershader_used->use();
		m_passinfo.ubershader_used->setAllUniforms();
		m_shadowmap->swap();
		m_postprocessfbo->clearAttachments();
		m_postprocessfbo->attachTexture(m_colormap, IRD::FrameBuffer::COLOR_ATTACHMENT_0);
		m_postprocessfbo->commitTextures(0);
		m_normalmap->bind(0);
		m_diffusemap->bind(1);
		m_specularmap->bind(2);
		m_depthmap->bind(3);
		m_shadowmap->bind(4);

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

		m_passinfo.ubershader_used->unset();
	}

	m_normalmap->release(0);
	m_diffusemap->release(1);
	m_specularmap->release(2);
	m_depthmap->release(3);
	m_postprocessfbo->swapTextures();
	m_postprocessfbo->release();

	m_postprocessfbo->clearAttachments();

	QList<Texture> input_textures;
	if(m_options.m_lightscattering_enabled)
	{
		input_textures.push_back(*m_lightscatteringmap_high);
		//input_textures.push_back(*m_colormap);
		m_passinfo.ubershader_used = m_lightscattering;
		postprocessPass(m_lightscatteringmap_low,input_textures,true);
		input_textures.clear();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_passinfo.ubershader_used = m_bloom;
		input_textures.push_back(*m_colormap);
		input_textures.push_back(*m_lightscatteringmap_low);
		postprocessPass(m_colormap,input_textures);
	}

	if(m_options.m_bloom_enabled)
	{
		glBlendFunc(GL_ONE, GL_ZERO);
		input_textures.push_back(*m_colormap);
		for(int i=0 ; i<3 ; i++) {
			m_passinfo.ubershader_used = m_vertical_blur;
			postprocessPass(m_bloommap,input_textures,true);
			m_passinfo.ubershader_used = m_horizontal_blur;
			input_textures.clear();
			input_textures.push_back(*m_bloommap);
			postprocessPass(m_bloommap,input_textures,true);
		}
	}
	else
	{
		//input_textures.push_back(*m_bloommap);
		input_textures.push_back(*m_colormap);
	}

	glBlendFunc(GL_ONE, GL_ONE);
	m_passinfo.ubershader_used = m_bloom;
	input_textures.push_front(*m_colormap);
	glClear(GL_COLOR_BUFFER_BIT);
	postprocessPass(0,input_textures);

	glBlendFunc(GL_ONE, GL_ZERO);
	glEnable(GL_DEPTH_TEST);
}

void RenderManager::renderForward(SceneGraph* sg, Viewpoint* viewpoint)
{
	/// Cleans
	clearTexture(m_bloommap);
	clearTexture(m_colormap);
	if(m_options.m_lightscattering_enabled)
	{
		clearTexture(m_lightscatteringmap_low);
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	QList< QPair<RenderTexture*, IRD::FrameBuffer::Attachment> > mrts;
	mrts.push_back(QPair<RenderTexture*, IRD::FrameBuffer::Attachment>(m_colormap, IRD::FrameBuffer::COLOR_ATTACHMENT_0));
	mrts.push_back(QPair<RenderTexture*, IRD::FrameBuffer::Attachment>(m_depthmap, IRD::FrameBuffer::DEPTH_STENCIL_ATTACHMENT));
	//mrts.push_back(QPair<RenderTexture*, IRD::FrameBuffer::Attachment>(m_depthmap, IRD::FrameBuffer::STENCIL_ATTACHMENT));
	RenderTarget srt(viewpoint, m_postprocessfbo, mrts , false, true);
	m_passinfo.ubershader_used = m_forward;
	m_passinfo.ubershader_used->resetParams();
	m_passinfo.lighting_enabled = true;
	m_passinfo.type = RenderPassInfo::FINAL_PASS;
	renderTarget(sg, srt);

	QList<Texture> input_textures;
	if(m_options.m_lightscattering_enabled)
	{
		//input_textures.push_back(*m_lightscatteringmap_high);
		input_textures.push_back(*m_colormap);
		m_passinfo.ubershader_used = m_lightscattering;
		postprocessPass(m_lightscatteringmap_low,input_textures,true);
		input_textures.clear();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_passinfo.ubershader_used = m_bloom;
		input_textures.push_back(*m_colormap);
		input_textures.push_back(*m_lightscatteringmap_low);
		postprocessPass(m_colormap,input_textures);
	}
	if(m_options.m_bloom_enabled)
	{
		glBlendFunc(GL_ONE, GL_ZERO);
		input_textures.push_back(*m_colormap);
		for(int i=0 ; i<3 ; i++) {
			m_passinfo.ubershader_used = m_vertical_blur;
			postprocessPass(m_bloommap,input_textures,true);
			m_passinfo.ubershader_used = m_horizontal_blur;
			input_textures.clear();
			input_textures.push_back(*m_bloommap);
			postprocessPass(m_bloommap,input_textures,true);
		}
	}
	else
	{
		//input_textures.push_back(*m_bloommap);
		input_textures.push_back(*m_colormap);
	}

	glBlendFunc(GL_ONE, GL_ONE);
	m_passinfo.ubershader_used = m_bloom;
	input_textures.push_front(*m_colormap);
	glClear(GL_COLOR_BUFFER_BIT);
	postprocessPass(0,input_textures);

	glBlendFunc(GL_ONE, GL_ZERO);
	glEnable(GL_DEPTH_TEST);
}

void RenderManager::renderShadowmaps(SceneGraph* sg)
{
	for(int i = 0 ; i<LIGHTING_MANAGER.managees().count() ; i++) {
		// Render depthmap
		Light* light = LIGHTING_MANAGER.managees().at(i);

		if(light->castsShadows()) {
			RenderTexture* depthtex = light->getRenderTexture();

			if(depthtex != 0) {
				FrameBufferObject fbo(depthtex->getWidth(), depthtex->getHeight(), false, false);
				RenderTarget srt(light, &fbo, depthtex, IRD::FrameBuffer::DEPTH_ATTACHMENT, false, false);
				debug("PASS_INFO","cast light " + QString().setNum(i));
				glCullFace(GL_FRONT);
				m_passinfo.setup_texture_matrices = true;
				m_passinfo.ubershader_used = m_depth;
				m_passinfo.type = RenderPassInfo::CAST_SHADOW_PASS;
				renderTarget(sg, srt);
				m_passinfo.setup_texture_matrices = false;
				glCullFace(GL_BACK);
			}
		}
	}
}

void RenderManager::render(double elapsed_time, SceneGraph* sg)
{
	m_context->makeCurrent();

	m_rendering = true;

	m_passinfo.context = m_context;
	m_passinfo.background_enabled = true;
	m_passinfo.setup_texture_matrices = false;
	m_passinfo.texturing_enabled = true;
	m_passinfo.lighting_enabled = true;
	m_passinfo.type = RenderPassInfo::FINAL_PASS;

	testViewportResize();

	// Frame Begin
	for(QVector<IRenderable*>::iterator it = registeredManagees.begin();
		it != registeredManagees.end();
		it++) {
		(*it)->frameBegin(elapsed_time);
	}

	debug("PASS_INFO","begin");

	Viewpoint* viewpoint = m_camera;
	if(m_camera == 0) {
		viewpoint = m_context->getViewpoint();
	}

	if(m_options.m_shadows_enabled) {
		renderShadowmaps(sg);
	}

	switch(m_options.m_pipeline)
	{
		case DEFERRED_PIPELINE:
			renderDeferred(sg,viewpoint);
			break;
		case FORWARD_PIPELINE:
			renderForward(sg,viewpoint);
			break;
		default:
			logError("Wrong pipeline value, will not render");
			abort();
	}

	// DrawDebug
	RenderTarget srt(viewpoint);
	drawDebug(sg,srt);

	int xpos = 0;
	for(int i = 0 ; i< m_debugTextures.size() ; i++)
	{
		debugDisplayTexture(m_debugTextures[i],xpos,0,256,256); xpos += 256;
	}

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

void RenderManager::renderTarget(SceneGraph* sg, RenderTarget& target)
{
	Viewpoint* viewpoint = target.getViewpoint();

	if(viewpoint == 0) {
		logError("No viewpoint to render from, you must set a camera");
		return;
	}

	target.bind();

	for(int pass_nb=0 ; pass_nb<target.getNbPass() ; pass_nb++) {

		target.setupPass(pass_nb);

		QList<IRenderable*> sss_renderables;
		QList<IRenderable*> transparent_renderables;

		if(m_context == 0) {
			return;
		}

		if(m_defaultBackground.type == Background::SKYBOX) {
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

		if(m_passinfo.lighting_enabled) {
			for(int i = 0 ; i<MAX_LIGHTS ; i++) {
				if(i<LIGHTING_MANAGER.managees().count())
				{
					// Render depthmap
					Light* light = LIGHTING_MANAGER.managees().at(i);
					light->sendParameters(i);

					m_passinfo.ubershader_used->setParamValue(UberShaderDefine::Type(UberShaderDefine::LIGHT_OMNI_0+i),light->getType() == Light::OMNI);
					m_passinfo.ubershader_used->setParamValue(UberShaderDefine::Type(UberShaderDefine::LIGHT_SPOT_0+i),light->getType() == Light::SPOT);
					m_passinfo.ubershader_used->setParamValue(UberShaderDefine::Type(UberShaderDefine::LIGHT_SUN_0+i),light->getType() == Light::SUN);
				}
				else
				{
					m_passinfo.ubershader_used->setParamValue(UberShaderDefine::Type(UberShaderDefine::LIGHT_OMNI_0+i),false);
					m_passinfo.ubershader_used->setParamValue(UberShaderDefine::Type(UberShaderDefine::LIGHT_SPOT_0+i),false);
					m_passinfo.ubershader_used->setParamValue(UberShaderDefine::Type(UberShaderDefine::LIGHT_SUN_0+i),false);
				}
			}
		}

		glDisable(GL_BLEND);
		for(QVector<IRenderable*>::iterator it = registeredManagees.begin();
			it != registeredManagees.end();
			it++) {

			if(m_passinfo.type == RenderPassInfo::CAST_SHADOW_PASS && !((*it)->castsShadows()))
				continue;
			if(m_passinfo.type == RenderPassInfo::RECEIVE_SHADOW_PASS && !((*it)->receivesShadows()))
				continue;

			if((*it)->usesSSS() && getSSSEnabled()){
				sss_renderables.push_back(*it); // SSS renderables are deferred for later rendering
			} else if((*it)->isTransparent()){
				transparent_renderables.push_back(*it); // Transparent renderables are deferred for later rendering
			} else {
				glPushMatrix();
				(*it)->render();
				glPopMatrix();
			}
		}

		for(QList<IRenderable*>::iterator it = sss_renderables.begin();
			it != sss_renderables.end();
			it++) {


			/// Prepass
			// Render SSS map
			m_sss_fbo->bind();
			m_sss_fbo->attachTexture(m_sssbuffer[0],IRD::FrameBuffer::COLOR_ATTACHMENT_0);
			m_sss_fbo->commitTextures(0);
			m_passinfo.ubershader_used->setParamValue(UberShaderDefine::SSS_PREPASS,true);
			glViewport(0,0,m_sssbuffer[0]->getWidth(), m_sssbuffer[0]->getHeight());
			glPushMatrix();
			(*it)->render();
			glPopMatrix();
			m_passinfo.ubershader_used->setParamValue(UberShaderDefine::SSS_PREPASS,false);
			m_sss_fbo->release();
			m_sssbuffer[0]->swap();

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glBlendFunc(GL_ONE, GL_ZERO);
			UberShader old_shader = m_passinfo.ubershader_used;
			QList<Texture> input_textures;
			for(int i=0 ; i<3 ; i++) {
				input_textures.clear();
				input_textures.push_back(*m_sssbuffer[qMax(i-1,0)]);
				m_passinfo.ubershader_used = m_vertical_blur;
				postprocessPassOnFBO(m_sssbuffer[i],input_textures,m_sss_fbo);
				input_textures.clear();
				input_textures.push_back(*m_sssbuffer[i]);
				m_passinfo.ubershader_used = m_horizontal_blur;
				postprocessPassOnFBO(m_sssbuffer[i],input_textures,m_sss_fbo);
			}
			m_passinfo.ubershader_used = old_shader;
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
/*
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			// HBlur SSS map
			m_sss_fbo->release();
			m_sss_fbo->bind();
			m_sss_fbo->attachTexture(m_sssbuffer,IRD::FrameBuffer::COLOR_ATTACHMENT_0);
			m_sss_fbo->commitTextures(0);
			m_horizontal_blur->use();
			m_horizontal_blur->setAllUniforms();
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
			m_sssbuffer->swap();
			// VBlur SSS map
			m_sss_fbo->release();
			m_sss_fbo->bind();
			m_sss_fbo->attachTexture(m_sssbuffer,IRD::FrameBuffer::COLOR_ATTACHMENT_0);
			m_sss_fbo->commitTextures(0);
			m_vertical_blur->use();
			m_vertical_blur->setAllUniforms();
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
			m_sssbuffer->swap();
			m_sss_fbo->release();
			// Restore matrices
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
*/
			// Final draw
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			target.bind();
			m_passinfo.ubershader_used->setParamValue(UberShaderDefine::SSS_FINAL,true);
			glViewport(0,0,m_screen_size->x(), m_screen_size->y());
			glPushMatrix();
			(*it)->render();
			glPopMatrix();
			m_passinfo.ubershader_used->setParamValue(UberShaderDefine::SSS_FINAL,false);
		}

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		for(QList<IRenderable*>::iterator it = transparent_renderables.begin();
			it != transparent_renderables.end();
			it++) {
			glPushMatrix();
			(*it)->render();
			glPopMatrix();
		}

		target.passDone();
	}

	target.release();
}

void RenderManager::drawDebug(SceneGraph* sg, RenderTarget& target)
{
	Viewpoint* viewpoint = target.getViewpoint();

	if(viewpoint == 0) {
		logError("No viewpoint to render from, you must set a camera");
		return;
	}

	target.bind();

	glClear(GL_DEPTH_BUFFER_BIT);


	for(int pass_nb=0 ; pass_nb<target.getNbPass() ; pass_nb++) {

		target.setupPass(pass_nb);

		if(m_context == 0) {
			return;
		}

		setupProjection(target,pass_nb);
		glLoadIdentity();
		viewpoint->applyTransform(pass_nb);

		if(m_drawDebug)	{
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			for(int i=0 ; i<sg->childCount() ; i++) {
				sg->child(i)->drawDebug(m_context,m_drawDebugFilter, true);
			}
			if(m_drawDebugFilter.draw_colliders)
			{
				PHYSICS_MANAGER.debugDraw();
			}
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_LIGHTING);
		}

		#ifdef WITH_TOOLS
			sg->getManipulator()->draw(m_context);
		#endif

		target.passDone();
	}

	target.release();
}

void RenderManager::clearTexture(RenderTexture* texture)
{
	debugGL("before clean");

	FrameBufferObject* fbo = m_postprocessfbo;

	if(texture == m_bloommap || texture == m_lightscatteringmap_low)
	{
		fbo = m_lowres_postprocessfbo;
	}

	fbo->bind();
	fbo->attachTexture(texture, IRD::FrameBuffer::COLOR_ATTACHMENT_0);

	fbo->commitTextures(0);
	glClear(GL_COLOR_BUFFER_BIT);
	fbo->swapTextures();

	fbo->commitTextures(0);
	glClear(GL_COLOR_BUFFER_BIT);
	fbo->swapTextures();

	fbo->release();

	debugGL("after clean");
}

void RenderManager::postprocessPass(RenderTexture* target_texture, QList<Texture> input_textures, bool lowres)
{
	QList< QPair<RenderTexture* , IRD::FrameBuffer::Attachment> > target_textures;

	if(target_texture != 0) {
		target_textures.push_back(QPair<RenderTexture*, IRD::FrameBuffer::Attachment>(target_texture, IRD::FrameBuffer::COLOR_ATTACHMENT_0));
	}

	postprocessPass(target_textures, input_textures, lowres);
}

void RenderManager::postprocessPass(QList< QPair<RenderTexture*, IRD::FrameBuffer::Attachment> > target_textures, QList<Texture> input_textures, bool lowres)
{
	if(lowres)
		postprocessPassOnFBO(target_textures, input_textures, m_lowres_postprocessfbo);
	else
		postprocessPassOnFBO(target_textures, input_textures, m_postprocessfbo);
}

void RenderManager::postprocessPassOnFBO(RenderTexture* target_texture, QList<Texture> input_textures, FrameBufferObject* fbo)
{
	QList< QPair<RenderTexture* , IRD::FrameBuffer::Attachment> > target_textures;

	if(target_texture != 0) {
		target_textures.push_back(QPair<RenderTexture*, IRD::FrameBuffer::Attachment>(target_texture, IRD::FrameBuffer::COLOR_ATTACHMENT_0));
	}

	postprocessPassOnFBO(target_textures, input_textures,fbo);
}

void RenderManager::postprocessPassOnFBO(QList< QPair<RenderTexture*, IRD::FrameBuffer::Attachment> > target_textures, QList<Texture> input_textures, FrameBufferObject* fbo)
{
	debugGL("before postprocessing");

	if(fbo == 0)
	{
		logError("Post processing with 0 fbo");
		return;
	}

	if(target_textures.size() > 0) {
		fbo->bind();
		for(int i = 0 ; i< target_textures.size() ; i++) {
			fbo->attachTexture(target_textures[i].first,
											target_textures[i].second);
		}
		fbo->commitTextures(0);
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

	if(target_textures.size() > 0) {
		width = fbo->getWidth();
		height = fbo->getHeight();
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

	if(target_textures.size() > 0) {
		fbo->swapTextures();
		fbo->release();
	}
}

void RenderManager::debugDisplayTexture(Texture texture, int x, int y, int width, int height)
{
	if(texture.isValid())
	{
		if(dynamic_cast<RenderTextureArray*>(*texture))
		{
			// Texture is a RenderTextureArray, use a special shader to render it unfolded
			ShaderProgram program = SHADER_PROGRAM_MANAGER.get("unfold_proxycubemap");
			program->use();
			program->setAllUniforms();
		}

		glClear(GL_DEPTH_BUFFER_BIT);

		glDisable(GL_LIGHTING);
		glColor4f(1.0,1.0,1.0,1.0);

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

		if(dynamic_cast<RenderTextureArray*>(*texture))
		{
			// Texture is a RenderTextureArray, use a special shader to render it unfolded
			SHADER_PROGRAM_MANAGER.get("unfold_proxycubemap")->unset();
		}
	}
}

void RenderManager::setCurrentCamera(Camera* cam)
{
	m_camera = cam;
	m_cameraChanged = true;

	#ifdef WITH_TOOLS
		getDebugView()->activeCameraChanged(m_camera);
	#endif
}

void RenderManager::setAmbient(Vector3f ambient)
{
	if(!m_scene_ambient)
		m_scene_ambient = new QVector3D();

	*m_scene_ambient = QVector3D(ambient.x, ambient.y, ambient.z);
}

void RenderManager::setDrawDebug(bool draw)
{
	m_drawDebug = draw;

	#ifdef WITH_TOOLS
		getDebugView()->setDrawDebug(draw);
	#endif
}
void RenderManager::setDrawDebugFilter(const DebugGizmosFilter& filter)
{
	m_drawDebugFilter = filter;

	#ifdef WITH_TOOLS
		getDebugView()->setDrawDebugFilter(filter);
	#endif
}

bool RenderManager::getDrawDebug() const
{
	return m_drawDebug;
}

RenderManager::DebugGizmosFilter RenderManager::getDrawDebugFilter() const
{
	return m_drawDebugFilter;
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

	if(viewpoint == 0) {
		logError("No viewpoint : can't apply background");
		return;
	}

	glClear(GL_DEPTH_BUFFER_BIT);
	glDepthMask(false);

	if(target.isOnScreen() || target.isStretchedToScreen()) {
		glViewport(0, 0, m_viewport_size.y, m_viewport_size.x);
	} else {
		int target_width = target.getWidth();
		int target_height = target.getHeight();
		glViewport(0, 0, target_width, target_height);
	}

	switch(m_defaultBackground.type)
	{
		case Background::COLOR :
		{
			m_passinfo.ubershader_used->setParamValue(UberShaderDefine::SKY, true);
			m_passinfo.ubershader_used->setParamValue(UberShaderDefine::COLORMAPPED, false);
			m_passinfo.ubershader_used->use();
			m_passinfo.ubershader_used->setAllUniforms();
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
				glLoadIdentity();
				gluOrtho2D(-1,1,-1,1);
				glDisable(GL_LIGHTING);
				glDisable(GL_TEXTURE_2D);
				//glColor4f(m_defaultBackground.color.r,m_defaultBackground.color.g,m_defaultBackground.color.b,1);
				GLfloat sky_color[] = {
					m_defaultBackground.color.r,
					m_defaultBackground.color.g,
					m_defaultBackground.color.b, 1.0
				};
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, sky_color);
				glBegin(GL_QUADS);
					glVertex2f(-1,-1);
					glVertex2f( 1,-1);
					glVertex2f( 1, 1);
					glVertex2f(-1, 1);
				glEnd();
				glEnable(GL_LIGHTING);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			break;
		}
		case Background::SINGLE_TEXTURE :
		{
			m_passinfo.ubershader_used->setParamValue(UberShaderDefine::SKY, true);
			m_passinfo.ubershader_used->setParamValue(UberShaderDefine::COLORMAPPED, true);
			m_passinfo.ubershader_used->use();
			m_passinfo.ubershader_used->setAllUniforms();

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
				glLoadIdentity();
				gluOrtho2D(-1,1,-1,1);
				glDisable(GL_LIGHTING);
				glEnable(GL_TEXTURE_2D);
				glColor4f(1,1,1,1);
				m_defaultBackground.textures[0]->bind();
				glBegin(GL_QUADS);
					glTexCoord2f(0,0); glVertex2f(-1,-1);
					glTexCoord2f(1,0); glVertex2f( 1,-1);
					glTexCoord2f(1,1); glVertex2f( 1, 1);
					glTexCoord2f(0,1); glVertex2f(-1, 1);
				glEnd();
				glDisable(GL_TEXTURE_2D);
				glEnable(GL_LIGHTING);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			break;
		}
		case Background::SKYBOX :
		{
			m_passinfo.ubershader_used->setParamValue(UberShaderDefine::SKY, true);
			m_passinfo.ubershader_used->setParamValue(UberShaderDefine::COLORMAPPED, true);
			m_passinfo.ubershader_used->use();
			m_passinfo.ubershader_used->setAllUniforms();

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

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				Transformf trans;
				trans.lookAt(m_defaultBackground.sunDirection);
				trans.glMultf();
				if(m_options.m_lightscattering_enabled)
				{
					m_defaultBackground.textures[5]->release();
					m_passinfo.ubershader_used->setParamValue(UberShaderDefine::LIGHT_SCATTERING_SUN, true);
					m_passinfo.ubershader_used->use();
					m_passinfo.ubershader_used->setAllUniforms();
					glBegin(GL_TRIANGLES);
						for(int i=0 ; i<36 ; i++)
						{
							glVertex3f( 0, 0, -9 );
							glVertex3f( fastSin(i*10), fastCos(i*10), -9);
							glVertex3f( fastSin((i+1)*10), fastCos((i+1)*10), -9);
						}
					glEnd();
					m_passinfo.ubershader_used->setParamValue(UberShaderDefine::LIGHT_SCATTERING_SUN, false);

					double mvmat[16];
					double projmat[16];
					glGetDoublev(GL_MODELVIEW_MATRIX, mvmat);
					glGetDoublev(GL_PROJECTION_MATRIX, projmat);
					Matrix4f mvmatbis(mvmat);
					Matrix4f projmatbis(projmat);
					Vector4f relativeSun(0,0,-1,1);

					relativeSun = mvmatbis*relativeSun;
					relativeSun = projmatbis*relativeSun;

					float pX = (1.0+relativeSun.x/relativeSun.w)/2;
					float pY = (1.0+relativeSun.y/relativeSun.w)/2;
					*m_sun_pos = QVector2D(pX, pY);
				}


			glPopMatrix();
			break;
		}
		default :
			break;
	}

	m_passinfo.ubershader_used->unset();
	m_passinfo.ubershader_used->setParamValue(UberShaderDefine::SKY, false);
	m_passinfo.ubershader_used->setParamValue(UberShaderDefine::COLORMAPPED, false);

	glDepthMask(true);
}

void RenderManager::addRenderTarget(RenderTarget *rt)
{
	if(rt != 0) {
		m_rts.push_back(rt);
	} else {
		logWarn("tried to add 0 render target");
	}
}

const QHash<QString, ShaderProgramData::UniformBase*>& RenderManager::getEngineUniforms()
{
	return m_engine_uniforms;
}

RenderManager::RenderPassInfo* RenderManager::getRenderPassInfo()
{
	RenderPassInfo* ret = 0;

	if(m_rendering) {
		ret = &m_passinfo;
	} else {
		logError("Asked for RenderPassInfo outside the rendering loop");
	}

	return ret;
}

Vector2i RenderManager::getCurrentViewportSize()
{
	return m_viewport_size;
}

void RenderManager::takeScreenshot(QString path)
{
	if(m_context)
	{
		m_context->takeScreenshot(path);
	}
}

GLWidget* RenderManager::getContext()
{
	return m_context;
}

void RenderManager::setBloomEnabled(bool enabled)
{
	if(!m_rendering) {
		if(m_options.m_bloom_enabled && !enabled)
		{
			if(m_bloommap && m_postprocessfbo)
				clearTexture(m_bloommap);
		}

		m_options.m_bloom_enabled = enabled;

		#ifdef WITH_TOOLS
			if(m_widget)
			{
				m_widget->setBloomEnabled(enabled);
			}
		#endif
	} else {
		logWarn("can't switch bloom enabled during rendering, TODO ; delay this kind of command");
	}
}

bool RenderManager::getBloomEnabled()
{
	return m_options.m_bloom_enabled;
}

void RenderManager::setShadowsEnabled(bool enabled)
{
	if(!m_rendering) {
		if(m_options.m_shadows_enabled && !enabled)
		{
			if(m_shadowmap && m_postprocessfbo)
				clearTexture(m_shadowmap);
		}

		m_options.m_shadows_enabled = enabled;

		#ifdef WITH_TOOLS
			if(m_widget)
			{
				m_widget->setShadowsEnabled(enabled);
			}
		#endif
	} else {
		logWarn("can't switch shadows enabled during rendering, TODO ; delay this kind of command");
	}
}

bool RenderManager::getShadowsEnabled()
{
	return m_options.m_shadows_enabled;
}

void RenderManager::setRenderPipeline(RenderPipeline pipeline)
{
	if(!m_rendering) {
		m_options.m_pipeline = pipeline;

		#ifdef WITH_TOOLS
			if(m_widget)
			{
				m_widget->setRenderPipeline(pipeline);
			}
		#endif
	} else {
		logWarn("can't switch pipeline during rendering, TODO ; delay this kind of command");
	}
}

RenderManager::RenderPipeline RenderManager::getRenderPipeline()
{
	return m_options.m_pipeline;
}

bool RenderManager::getSSSEnabled()
{
	return m_options.m_sss_enabled;
}

void RenderManager::setSSSEnabled(bool enabled)
{
	m_options.m_sss_enabled = enabled;

	#ifdef WITH_TOOLS
		if(m_widget)
		{
			m_widget->setSSSEnabled(enabled);
		}
	#endif
}

bool RenderManager::getNormalMappingEnabled()
{
	return m_options.m_normalmapping_enabled;
}

void RenderManager::setNormalMappingEnabled(bool enabled)
{
	m_options.m_normalmapping_enabled = enabled;

	#ifdef WITH_TOOLS
		if(m_widget)
		{
			m_widget->setNormalMappingEnabled(enabled);
		}
	#endif
}

bool RenderManager::getSpecularMappingEnabled()
{
	return m_options.m_specularmapping_enabled;
}

void RenderManager::setSpecularMappingEnabled(bool enabled)
{
	m_options.m_specularmapping_enabled = enabled;

	#ifdef WITH_TOOLS
		if(m_widget)
		{
			m_widget->setSpecularMappingEnabled(enabled);
		}
	#endif
}

bool RenderManager::getLightScatteringEnabled()
{
	return m_options.m_lightscattering_enabled;
}

void RenderManager::setLightScatteringEnabled(bool enabled)
{
	m_options.m_lightscattering_enabled = enabled;

	#ifdef WITH_TOOLS
		if(m_widget)
		{
			m_widget->setLightScatteringEnabled(enabled);
		}
	#endif
}

void RenderManager::addDebugTexture(Texture texture)
{
	if(texture.isValid())
		m_debugTextures.push_back(texture);
}

void RenderManager::clearDebugTextures()
{
	m_debugTextures.clear();
}

IRD::iRenderDevice* RenderManager::getRenderDevice()
{
	return m_device;
}

void RenderManager::setTextureMatrix(QMatrix4x4 mat, int passNb)
{
	m_texture_matrices[passNb] = mat;
}

#ifdef WITH_TOOLS

RenderWidget* RenderManager::getDebugView()
{
	if(m_widget == 0)
		m_widget = new RenderWidget();

	return m_widget;
}

void RenderManager::widgetDestroyed()
{
	m_widget = 0;
}

#endif

