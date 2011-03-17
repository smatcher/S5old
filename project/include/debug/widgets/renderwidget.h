#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QWidget>
#include <QLayout>

class Camera;
class RenderWidget : public QWidget
{
    Q_OBJECT

public :
    RenderWidget();
    virtual ~RenderWidget();

    void cameraAdded(Camera* cam);

    QSize sizeHint() const;

public slots :
    void drawDebugChanged(int state);

private:
    QVBoxLayout* camerasLayout;
};

#endif // RENDERWIDGET_H
