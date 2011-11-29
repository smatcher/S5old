#ifndef RESOURCEWIDGET_H
#define RESOURCEWIDGET_H

#include <QWidget>

class ResourceData;

class ResourceWidget : public QWidget
{
	Q_OBJECT

private:
	ResourceData& m_resource;

public:
	ResourceWidget(ResourceData& resource);
	virtual ~ResourceWidget();

	virtual void updateData();
};

#endif // RESOURCEWIDGET_H
