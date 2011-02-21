#ifndef SCENEGRAPHMODEL_H
#define SCENEGRAPHMODEL_H

#include "include/core/scenegraph/scenegraph.h"
#include "QAbstractItemModel"

class SceneGraph;

class SceneGraphModel : public QAbstractItemModel
{
	Q_OBJECT

private :
	SceneGraph* m_item;

	ParentOf<Node>* getItem(const QModelIndex &index) const;
public :
	SceneGraphModel(SceneGraph* item);
	~SceneGraphModel();

	QModelIndex index(int row, int column, const QModelIndex &parent) const;
	QModelIndex parent(const QModelIndex&) const;
	int rowCount(const QModelIndex&) const;
	int columnCount(const QModelIndex&) const;
	QVariant data(const QModelIndex&, int role) const;

};

#endif // SCENEGRAPHMODEL_H
