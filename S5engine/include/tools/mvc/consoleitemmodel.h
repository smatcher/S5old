#ifndef CONSOLEITEMMODEL_H
#define CONSOLEITEMMODEL_H

#include <QList>
#include <QIcon>
#include <QFont>
#include <QDir>
#include <QAbstractListModel>

struct ConsoleItem
{
	enum ConsoleItemType
	{
		Input,
		Echo,
		Debug,
		Info,
		Warning,
		Error
	};

	ConsoleItem(ConsoleItemType type, const QIcon& icon, const QString& msg)
	{
		this->msg = msg;
		this->type = type;
		this->icon = icon;

		hasFile = false;
		hasTopic = false;
	}

	void setFile(QDir file, int line, QString function)
	{
		this->file = file;
		this->line = line;
		this->function = function;

		hasFile = true;
	}

	void setTopic(QString topic)
	{
		this->topic = topic;

		hasTopic = true;
	}

	ConsoleItemType type;
	QIcon  icon;
	QString msg;

	bool hasTopic;
	QString topic;

	bool hasFile;
	QDir file;
	int line;
	QString function;
};

class ConsoleItemModel : public QAbstractListModel
{
	Q_OBJECT

public:

	enum Roles { Type = Qt::UserRole, Message, File, Line, Function, Topic, Icon };

	ConsoleItemModel();
	virtual ~ConsoleItemModel();

	void insertItem(ConsoleItem& item);

	virtual int rowCount(const QModelIndex &parent) const;

	virtual QVariant data(const QModelIndex& parent, int role) const;

	int sizeOfTopic(const QFont &font);

private:
	QList<ConsoleItem> m_items;

	int m_maxSizeOfTopic;
	int m_lastMaxTopicIndex;
	QFont m_topicMeasurementFont;
};

#endif // CONSOLEITEMMODEL_H
