#ifndef FOLDABLEGROUP_H
#define FOLDABLEGROUP_H

#include <QWidget>
#include <QLayout>

class FoldableGroup : QWidget
{
	Q_OBJECT

protected :
	QLayout* m_layout;
	QWidget* m_button;
	QWidget* m_label;
	QWidget* m_group;

public :
	FoldableGroup(const QString& name = "Foldable exemple");
	virtual ~FoldableGroup();

	void setGroup(QWidget* group);
};

#endif // FOLDABLEGROUP_H
