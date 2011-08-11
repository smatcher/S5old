#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QWidget>
#include <QLayout>
#include <QCheckBox>

class Camera;
class CameraRadioButton;
class RenderWidget : public QWidget
{
	Q_OBJECT

public :
	RenderWidget();
	virtual ~RenderWidget();

	void cameraAdded(Camera* cam);
	void cameraRemoved(CameraRadioButton *radio);
	void activeCameraChanged(Camera* cam);

	void setDrawDebug(bool draw);

	QSize sizeHint() const;

public slots :
	void drawDebugChanged(int state);

private:
	QVBoxLayout* camerasLayout;
	CameraRadioButton* editorCam;
	QCheckBox* m_draw_debug_radio;
};

#endif // RENDERWIDGET_H
