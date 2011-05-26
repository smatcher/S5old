#ifndef PROPERTIESPANEL_H
#define PROPERTIESPANEL_H

#include <QScrollArea>
#include <QLayout>
#include <QItemSelection>
#include "tools/widgets/nodewidget.h"

class PropertiesPanel : public QScrollArea
{
	Q_OBJECT

private :
	NodeWidget* m_widget;
public :
	PropertiesPanel();
	virtual ~PropertiesPanel();

	QSize sizeHint() const;

	virtual void updateData();

public slots :
	void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
};

#endif // PROPERTIESPANEL_H
