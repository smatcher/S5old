#ifndef RESOURCEWIDGET_H
#define RESOURCEWIDGET_H

#include <QGroupBox>

class ResourceData;
class QVBoxLayout;
class QLabel;
class QPushButton;

class ResourceWidget : public QGroupBox
{
	Q_OBJECT

protected:
	ResourceData& m_resource;
	QVBoxLayout*  m_layout;
	QLabel*       m_status;
	QPushButton*  m_reloadButton;

public:
	ResourceWidget(ResourceData& resource);
	virtual ~ResourceWidget();

	virtual void updateData();

public slots:
	void reloadResource();
};

#endif // RESOURCEWIDGET_H
