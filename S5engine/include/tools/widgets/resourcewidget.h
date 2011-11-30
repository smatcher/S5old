#ifndef RESOURCEWIDGET_H
#define RESOURCEWIDGET_H

#include <QGroupBox>

class ResourceData;
class QVBoxLayout;

class ResourceWidget : public QGroupBox
{
	Q_OBJECT

protected:
	ResourceData& m_resource;
	QVBoxLayout*  m_layout;

public:
	ResourceWidget(ResourceData& resource);
	virtual ~ResourceWidget();

	virtual void updateData();
};

#endif // RESOURCEWIDGET_H
