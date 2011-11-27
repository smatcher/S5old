#include "tools/mvc/consoleitemmodel.h"

#include <QFontMetrics>

ConsoleItemModel::ConsoleItemModel() : QAbstractListModel(),
	m_maxSizeOfTopic(0),
	m_lastMaxTopicIndex(0)
{
}

ConsoleItemModel::~ConsoleItemModel()
{
}

void ConsoleItemModel::insertItem(ConsoleItem &item)
{
	beginInsertRows(QModelIndex(), m_items.count(), m_items.count());
	m_items.push_back(item);
	endInsertRows();
}

int ConsoleItemModel::rowCount(const QModelIndex &parent) const
{
	if(parent.isValid())
		return 0;
	else
		return m_items.count();
}

QVariant ConsoleItemModel::data(const QModelIndex& parent, int role) const
{
	if(parent.isValid())
	{
		int row = parent.row();
		if(row >= 0 && row < m_items.count())
		{
			const ConsoleItem& item = m_items.at(row);
			if(role == Type){
				return item.type;
			} else if(role == Icon) {
				return item.icon;
			} else if(role == Message) {
				return item.msg;
			} else if(role == File) {
				if(item.hasFile)
					return item.file.path();
				else
					return QVariant();
			} else if(role == Line) {
				if(item.hasFile)
					return item.line;
				else
					return QVariant();
			} else if(role == Function) {
					if(item.hasFile)
						return item.function;
					else
						return QVariant();
			} else if(role == Topic) {
					if(item.hasTopic)
						return item.topic;
					else
						return QVariant();
			} else if(role == Qt::DisplayRole) {
					return item.msg;
			} else if(role == Qt::DecorationRole) {
					return item.icon;
			}
		}
	}
	return QVariant();
}

int ConsoleItemModel::sizeOfTopic(const QFont &font)
{
	 int count = m_items.count();
	 if (count == 0)
		  return 0;

	 if (m_maxSizeOfTopic > 0 && font == m_topicMeasurementFont && m_lastMaxTopicIndex == count - 1)
		  return m_maxSizeOfTopic;

	 QFontMetrics fm(font);
	 m_topicMeasurementFont = font;
	 for (int i = m_lastMaxTopicIndex; i < count; ++i) {
		  m_maxSizeOfTopic = qMax(m_maxSizeOfTopic, fm.width(m_items.at(i).topic));
	 }
	 m_lastMaxTopicIndex = count - 1;
	 return m_maxSizeOfTopic;
}

