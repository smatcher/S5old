#include "tools/widgets/texturewidget.h"
#include "core/graphics/texture.h"

#include "core/managers/rendermanager.h"
#include "core/framework/glwidget.h"

#include <QLabel>

TextureWidget::TextureWidget(TextureData &resource) : ResourceWidget(resource), m_preview(0), m_resolution_info(0), m_openglId_info(0)
{
	if(resource.m_state == ResourceData::STATE_LOADED)
	{
		TextureData* texture = (TextureData*)&resource;

		m_debug = new QPushButton("Add to debug");
		m_layout->addWidget(m_debug);
		m_resolution_info = new QLabel("Resolution : "+QString().setNum(resource.m_width)+"x"+QString().setNum(resource.m_height));
		m_layout->addWidget(m_resolution_info);
		m_openglId_info = new QLabel("OpenGL ID : "+QString().setNum(0/*texture->getGLId()*/));
		m_layout->addWidget(m_openglId_info);
		m_preview = new TexturePreview((QGLWidget*)RENDER_MANAGER.getContext(),0);//texture->getGLId());
		m_layout->addWidget(m_preview);

		m_layout->addStretch(1);

		connect(m_debug,SIGNAL(clicked()),this,SLOT(addToDebug()));
	}
}

TextureWidget::~TextureWidget()
{
}

TextureWidget::TexturePreview::TexturePreview(QGLWidget *shared, GLint glId) : QGLWidget(0,shared)
{
	this->glId = glId;
	setMinimumHeight(200);
	setMinimumWidth(200);
}

void TextureWidget::TexturePreview::setGLId(GLint glId)
{
	this->glId = glId;
	updateGL();
}

void TextureWidget::TexturePreview::paintGL()
{
	//drawTexture(this->geometry(),glId);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, glId);
	/*
	glViewport(0,0,200,200);
	gluOrtho2D(0,1,0,1);
	*/
	glBegin(GL_QUADS);
		glTexCoord2d(0.0,0.0); glVertex2d(-1.0,-1.0);
		glTexCoord2d(1.0,0.0); glVertex2d( 1.0,-1.0);
		glTexCoord2d(1.0,1.0); glVertex2d( 1.0, 1.0);
		glTexCoord2d(0.0,1.0); glVertex2d(-1.0, 1.0);
	glEnd();
	swapBuffers();
}

void TextureWidget::updateData()
{
	ResourceWidget::updateData();
	TextureData* texture = (TextureData*)&m_resource;

	if(texture->m_state == ResourceData::STATE_LOADED) {

		if(m_resolution_info) {
			m_resolution_info->setText("Resolution : "+QString().setNum(texture->m_width)+"x"+QString().setNum(texture->m_height));
		} else {
			m_resolution_info = new QLabel("Resolution : "+QString().setNum(texture->m_width)+"x"+QString().setNum(texture->m_height));
			m_layout->addWidget(m_resolution_info);
		}

		if(m_openglId_info) {
			m_openglId_info->setText("OpenGL ID : "+QString().setNum(0/*texture->getGLId()*/));
		} else {
			m_openglId_info = new QLabel("OpenGL ID : "+QString().setNum(0/*texture->getGLId()*/));
			m_layout->addWidget(m_openglId_info);
		}
		if(m_preview) {
			m_preview->setGLId(0/*texture->getGLId()*/);
		} else {
			m_preview = new TexturePreview((QGLWidget*)RENDER_MANAGER.getContext(),0/*texture->getGLId()*/);
			m_layout->addWidget(m_preview);
			m_layout->addStretch(1);
		}
	}
}

void TextureWidget::addToDebug()
{
	TextureData* texture = (TextureData*)&m_resource;
	RENDER_MANAGER.addDebugTexture(Texture(*texture));
}
