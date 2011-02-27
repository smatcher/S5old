#ifndef PROPERTIESPANEL_H
#define PROPERTIESPANEL_H

#include <QWidget>
#include <QLayout>
#include <QItemSelection>
#include "include/debug/widgets/nodewidget.h"

class PropertiesPanel : public QWidget
{
	Q_OBJECT

private :
	QLayout* m_layout;
	NodeWidget* m_widget;
public :
	PropertiesPanel();
	virtual ~PropertiesPanel();

	void setWidget(NodeWidget* widget);

	QSize sizeHint() const;

public slots :
	void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
};

#endif // PROPERTIESPANEL_H