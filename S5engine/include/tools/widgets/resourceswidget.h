#ifndef RESOURCESWIDGET_H
#define RESOURCESWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QTabWidget>
#include <QAbstractListModel>
#include <QListView>
#include <QHBoxLayout>

class ResourcesWidget : public QWidget
{
	Q_OBJECT

public:
	ResourcesWidget();
	virtual ~ResourcesWidget();

public slots:
	void currentTabChanged(int index);
	void currentResourceChanged ( const QItemSelection & current, const QItemSelection & previous );

private:

	enum TABS
	{
		MATERIALS = 0,
		MESHES,
		SHADERS,
		SOUNDS,
		TEXTURES,
		NB_TABS
	};
	static QString tabNames[NB_TABS];

	int m_current_tab;
	QTabWidget*         m_tab;
	QFrame*             m_frames[NB_TABS];
	QListView*          m_listviews[NB_TABS];
	QAbstractListModel* m_listmodels[NB_TABS];
};

#endif // RESOURCESWIDGET_H
