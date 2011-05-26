#ifndef SCENEGRAPHMODEL_H
#define SCENEGRAPHMODEL_H

#include "core/scenegraph/scenegraph.h"
#include "core/scenegraph/node.h"
#include <QAbstractItemModel>
#include <QItemSelectionModel>

class SceneGraph;
class Node;

class SceneGraphModel : public QAbstractItemModel
{
	Q_OBJECT

private :
	SceneGraph* m_item;

	Node* getNode(const QModelIndex &index) const;
public :
	SceneGraphModel(SceneGraph* item);
	virtual ~SceneGraphModel();

	QModelIndex index(int row, int column, const QModelIndex &parent) const;
	QModelIndex parent(const QModelIndex&) const;
	int rowCount(const QModelIndex&) const;
	int columnCount(const QModelIndex&) const;
	QVariant data(const QModelIndex&, int role) const;
	virtual bool event(QEvent* evt);
};

#endif // SCENEGRAPHMODEL_H
