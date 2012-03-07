#include "tools/widgets/consolewidget.h"
#include "tools/mvc/consoleitemmodel.h"
#include "tools/mvc/consoleitemdelegate.h"
#include "core/managers/commandmanager.h"
#include "core/log/log.h"

#include <QKeyEvent>
#include <QEvent>
#include <QList>
#include <QAbstractListModel>

ConsoleWidget* ConsoleWidget::m_instance = 0;

ConsoleWidget::ConsoleWidget()
{
	if(!m_instance)
	{
		m_instance = this;
	}
	else
	{
		logError("Only one console widget is allowed");
		abort();
	}

	m_historyId = -1;
	m_content_field = new QListView();
	m_content_model = new ConsoleItemModel();
	m_input_field = new ConsoleInputField();
	m_layout = new QVBoxLayout();
	m_layout->addWidget(m_content_field);
	m_layout->addWidget(m_input_field);
	setLayout(m_layout);

	ConsoleItemDelegate* delegate = new ConsoleItemDelegate(this);

	m_content_field->setModel(m_content_model);
	m_content_field->setItemDelegate(delegate);
	m_content_field->setAlternatingRowColors(true);

	m_ok = QIcon("media/icons/ok.png");
	m_nok = QIcon("media/icons/nok.png");
	m_info = QIcon("media/icons/info.png");
	m_warn = QIcon("media/icons/warn.png");
	m_error = QIcon("media/icons/error.png");
	m_input = QIcon("media/icons/input.png");
	m_debug = QIcon("media/icons/debug.png");

	QObject::connect(m_input_field,SIGNAL(returnPressed()),this,SLOT(newCommand()));

	connect(m_content_field->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
					delegate, SLOT(currentChanged(QModelIndex,QModelIndex)));

	COMMAND_MANAGER.registerConsole(this);
}

ConsoleWidget::~ConsoleWidget()
{
	m_instance = 0;
}

void ConsoleWidget::echo(QString msg)
{
	ConsoleItem item(ConsoleItem::Echo, QIcon(), msg);
	m_content_model->insertItem(item);
}

void ConsoleWidget::newCommand()
{
	QString cmd = m_input_field->text();

	ConsoleItem item(ConsoleItem::Input, m_input, cmd);
	m_content_model->insertItem(item);

	bool retval = COMMAND_MANAGER.runCommand(cmd);

	m_input_field->clear();
	m_historyId = -1;
	m_history.push_back(cmd);

	if(retval)
	{
		item = ConsoleItem(ConsoleItem::Echo, m_ok, "command returned ok");
	}
	else
	{
		item = ConsoleItem(ConsoleItem::Echo, m_nok, "command returned not ok");
	}

	m_content_model->insertItem(item);
	m_content_field->scrollToBottom();

}

void ConsoleWidget::historyNext()
{
	if(m_historyId < 0)
		m_historyId = m_history.count() -1;
	else if(m_historyId < (m_history.count() - 1))
		m_historyId++;

	if(m_history.count() > m_historyId)
		m_input_field->setText(m_history.at(m_historyId));
}

void ConsoleWidget::historyPrevious()
{
	if(m_historyId < 0)
		m_historyId = m_history.count() -1;
	else
		m_historyId--;

	if(m_historyId < 0)
		m_historyId = 0;

	if(m_history.count() > m_historyId)
		m_input_field->setText(m_history.at(m_historyId));
}

void ConsoleWidget::autocomplete()
{
	QStringList suggestions = COMMAND_MANAGER.autocomplete(m_input_field->text());

	if(suggestions.count() == 1)
	{
		m_input_field->setText(suggestions.at(0));
	}
	else if(suggestions.count() > 1)
	{
		QString msg = "suggested commands :";
		for(int i=0 ; i<suggestions.count() ; i++) {
			msg.append("\n" + suggestions.at(i));
		}
		echo(msg);
		m_content_field->scrollToBottom();
	}
}

void ConsoleWidget::ConsoleInputField::keyPressEvent(QKeyEvent * event)
{
	if(event->key() == Qt::Key_Up)
	{
		ConsoleWidget::m_instance->historyPrevious();
	}
	else if(event->key() == Qt::Key_Down)
	{
		ConsoleWidget::m_instance->historyNext();
	}
	else
	{
		QLineEdit::keyPressEvent(event);
	}
}

bool ConsoleWidget::ConsoleInputField::event(QEvent * event)
{
	if(event->type() == QEvent::KeyPress)
	{
		QKeyEvent* kevent = dynamic_cast<QKeyEvent*>(event);
		if(kevent && kevent->key() == Qt::Key_Tab)
		{
			m_instance->autocomplete();
			return true;
		}
	}
	return QLineEdit::event(event);
}

void ConsoleWidget::log(Log::LogItem& log)
{
	ConsoleItem* item = 0;

	switch(log.type)
	{
		case Log::LOG_DEBUG:
			item = new ConsoleItem(ConsoleItem::Info, m_debug, log.message);
			item->type = ConsoleItem::Debug;
			break;
		case Log::LOG_INFO:
			item = new ConsoleItem(ConsoleItem::Info, m_info, log.message);
			item->type = ConsoleItem::Info;
			break;
		case Log::LOG_WARN:
			item = new ConsoleItem(ConsoleItem::Info, m_warn, log.message);
			item->type = ConsoleItem::Warning;
			break;
		case Log::LOG_ERROR:
			item = new ConsoleItem(ConsoleItem::Info, m_error, log.message);
			item->type = ConsoleItem::Error;
			break;
		default:
			item = new ConsoleItem(ConsoleItem::Info, QIcon(), log.message);
			item->type = ConsoleItem::Echo;
			break;
	}

	if(log.has_file)
		item->setFile(log.file, log.line, log.function);

	if(log.has_topic)
		item->setTopic(log.topic);

	if(m_content_model)
		m_content_model->insertItem(*item);

	delete item;
}
