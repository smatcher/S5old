#ifndef MESHRENDERERWIDGET_H
#define MESHRENDERERWIDGET_H

#include "tools/widgets/propertywidget.h"
#include <QLineEdit>

class MeshRenderer;

class MeshRendererWidget : public PropertyWidget
{
public :
	MeshRendererWidget(MeshRenderer* property);
	virtual ~MeshRendererWidget();

	virtual void updateData();

private :
	QLineEdit *m_material,  *m_texture,  *m_mesh;
};

#endif // MESHRENDERERWIDGET_H
