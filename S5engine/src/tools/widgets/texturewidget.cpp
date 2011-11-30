#include "tools/widgets/texturewidget.h"
#include "core/graphics/texture.h"

#include "core/managers/rendermanager.h"
#include "core/framework/glwidget.h"

#include <QLabel>

TextureWidget::TextureWidget(TextureData &resource) : ResourceWidget(resource), m_preview(NULL), m_resolution_info(NULL)
{
	if(resource.m_state == ResourceData::STATE_LOADED)
	{
		TextureData* texture = (TextureData*)&resource;
		QWidget* wid;

		m_resolution_info = new QLabel("Resolution : "+QString().setNum(resource.m_width)+"x"+QString().setNum(resource.m_height));
		m_layout->addWidget(m_resolution_info);
		wid = new QLabel("OpenGL ID : "+QString().setNum(texture->getGLId()));
		m_layout->addWidget(wid);
		m_preview = new TexturePreview((QGLWidget*)RENDER_MANAGER.getContext(),texture->getGLId());
		m_preview->setMinimumHeight(200);
		m_preview->setMinimumWidth(200);
		m_layout->addWidget(m_preview);

		m_layout->addStretch(1);
	}
}

TextureWidget::~TextureWidget()
{
}

TextureWidget::TexturePreview::TexturePreview(QGLWidget *shared, GLint glId) : QGLWidget(NULL,shared)
{
	this->glId = glId;
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
	if(m_resolution_info)
	{
		TextureData* texture = (TextureData*)&m_resource;
		m_resolution_info->setText("Resolution : "+QString().setNum(texture->m_width)+"x"+QString().setNum(texture->m_height));
	}

	if(m_preview)
		m_preview->update();
}
