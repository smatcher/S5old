#ifndef MESHWIDGET_H
#define MESHWIDGET_H

#include "tools/widgets/resourcewidget.h"
#include "core/maths/transform.h"

#include <QGLWidget>

class QLabel;
class MeshData;

class MeshWidget : public ResourceWidget
{

	class MeshPreview : public QGLWidget
	{
	public:
		MeshData* m_mesh;
		Transformf m_transform;

	private:
		int m_lastx;
		int m_lasty;
		bool m_last_pressed;

	public :
		MeshPreview(QGLWidget* shared, MeshData* mesh);
		virtual void paintGL();

	protected:
		virtual void mouseMoveEvent(QMouseEvent *event);
		virtual void mousePressEvent(QMouseEvent *event);
		virtual void mouseReleaseEvent(QMouseEvent *event);
	};

	MeshPreview* m_preview;
	QLabel* m_info;

public:
	MeshWidget(MeshData& resource);
	virtual ~MeshWidget();
};

#endif // MESHWIDGET_H
