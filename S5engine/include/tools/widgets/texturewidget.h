#ifndef TEXTUREWIDGET_H
#define TEXTUREWIDGET_H

#include "tools/widgets/resourcewidget.h"

#include <QGLWidget>

class TextureData;
class QLabel;

class TextureWidget : public ResourceWidget
{
private:

	class TexturePreview : public QGLWidget
	{
	private:
		GLint glId;
	public :
		TexturePreview(QGLWidget* shared, GLint glId);
		virtual void paintGL();
		void setGLId(GLint glId);
	};

	TexturePreview* m_preview;
	QLabel* m_resolution_info;
	QLabel* m_openglId_info;

public:
	TextureWidget(TextureData& resource);
	virtual ~TextureWidget();

	virtual void updateData();
};

#endif // TEXTUREWIDGET_H
