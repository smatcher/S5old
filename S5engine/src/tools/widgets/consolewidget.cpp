#include "tools/widgets/consolewidget.h"
#include "core/managers/commandmanager.h"
#include "core/log/log.h"

#include <QKeyEvent>
#include <QEvent>

ConsoleWidget* ConsoleWidget::m_instance = NULL;

void ConsoleWidget::MsgHandler(QtMsgType type, const char *msg)
{
	if(m_instance)
	{
		QListWidgetItem* item;
		switch (type) {
			case QtDebugMsg:
				item = new QListWidgetItem();
				item->setText(msg);
				item->setIcon(m_instance->m_info);
				m_instance->m_content_field->addItem(item);
				break;
			case QtWarningMsg:
				item = new QListWidgetItem();
				item->setText(msg);
				item->setIcon(m_instance->m_warn);
				m_instance->m_content_field->addItem(item);
				break;
			case QtCriticalMsg:
				item = new QListWidgetItem();
				item->setText(msg);
				item->setIcon(m_instance->m_error);
				m_instance->m_content_field->addItem(item);
				break;
			default:
				break;
		}

		if(m_instance->m_content_field->isVisible())
		{
			m_instance->m_content_field->scrollToBottom();
		}
	}
	switch (type) {
		case QtDebugMsg:
			fprintf(stdout, "%s\n", msg);
			fflush(stdout);
			break;
		case QtWarningMsg:
			fprintf(stderr, "%s\n", msg);
			fflush(stderr);
			break;
		case QtCriticalMsg:
			fprintf(stderr, "%s\n", msg);
			fflush(stderr);
			break;
		case QtFatalMsg:
			fprintf(stderr, "%s\n", msg);
			abort();
	}
}

ConsoleWidget::ConsoleWidget()
{
	if(!m_instance)
	{
		m_instance = this;
		qInstallMsgHandler(ConsoleWidget::MsgHandler);

		m_historyId = -1;
		m_content_field = new QListWidget();
		m_input_field = new ConsoleInputField();
		m_layout = new QVBoxLayout();
		m_layout->addWidget(m_content_field);
		m_layout->addWidget(m_input_field);
		setLayout(m_layout);

		m_ok = QIcon("media/icons/ok.png");
		m_nok = QIcon("media/icons/nok.png");
		m_info = QIcon("media/icons/info.png");
		m_warn = QIcon("media/icons/warn.png");
		m_error = QIcon("media/icons/error.png");

		QObject::connect(m_input_field,SIGNAL(returnPressed()),this,SLOT(newCommand()));
	}
	else
	{
		logError("Only one console widget is allowed");
		abort();
	}
}

ConsoleWidget::~ConsoleWidget()
{
	m_instance = NULL;
}

void ConsoleWidget::newCommand()
{
	QString cmd = m_input_field->text();

	bool retval = COMMAND_MANAGER.runCommand(cmd);

	m_input_field->clear();
	m_historyId = -1;
	m_history.push_back(cmd);

	QListWidgetItem* item = new QListWidgetItem();
	item->setText(cmd);

	if(retval)
		item->setIcon(m_ok);
	else
		item->setIcon(m_nok);

	m_content_field->addItem(item);
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
		m_input_field->setText(suggestions.at(0));
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
