#include "include/core/scenegraph/scenegraphmodel.h"

SceneGraphModel::SceneGraphModel(SceneGraph* item)
{
	m_item = item;
}

SceneGraphModel::~SceneGraphModel()
{
	m_item->clearDebug();
}

ParentOf<Node>* SceneGraphModel::getItem(const QModelIndex &index) const
{
	if (index.isValid())
	{
		ParentOf<Node> *item = static_cast<ParentOf<Node>*>(index.internalPointer());
		if (item) return item;
	}
	return m_item;
}

QModelIndex SceneGraphModel::index(int row, int column, const QModelIndex& parent) const
{
	if (parent.isValid() && parent.column() != 0)
		return QModelIndex();

	ParentOf<Node> *parentItem = getItem(parent);

	Node *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex SceneGraphModel::parent(const QModelIndex& index) const
{
	if (!index.isValid())
		return QModelIndex();

	Node *childItem = static_cast<Node*>(getItem(index));
	ParentOf<Node> *parentItem = childItem->parent();

	if (parentItem == m_item)
		return QModelIndex();

	return createIndex(((Node*)parentItem)->childNumber(), 0, parentItem);
}

int SceneGraphModel::rowCount(const QModelIndex& parent) const
{
	ParentOf<Node> *parentItem = getItem(parent);

	return parentItem->childCount();
}

int SceneGraphModel::columnCount(const QModelIndex&) const
{
	return 1;
}

QVariant SceneGraphModel::data(const QModelIndex& parent, int role) const
{
	if(role == Qt::DisplayRole)
	{
		ParentOf<Node>* n = getItem(parent);
		if(n != m_item) return QVariant(static_cast<Node*>(n)->getName());
	}
	return QVariant();
}
