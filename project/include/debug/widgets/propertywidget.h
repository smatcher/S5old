#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include <QWidget>

class IProperty;

class PropertyWidget : public QWidget
{
	Q_OBJECT

private :
	IProperty* m_property;

public :
	PropertyWidget(IProperty* property);
	virtual ~PropertyWidget();
};

#endif // PROPERTYWIDGET_H
