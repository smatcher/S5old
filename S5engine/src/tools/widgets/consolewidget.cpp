#include "tools/widgets/consolewidget.h"
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
				item->setIcon(QIcon("media/icons/info.png"));
				m_instance->m_content_field->addItem(item);
				break;
			case QtWarningMsg:
				item = new QListWidgetItem();
				item->setText(msg);
				item->setIcon(QIcon("media/icons/warn.png"));
				m_instance->m_content_field->addItem(item);
				break;
			case QtCriticalMsg:
				item = new QListWidgetItem();
				item->setText(msg);
				item->setIcon(QIcon("media/icons/error.png"));
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
	m_historyId = -1;
	m_history.push_back(m_input_field->text());
	m_content_field->addItem(QString("console not implemented yet ignoring command : ") + m_input_field->text());
	m_content_field->scrollToBottom();
	m_input_field->clear();
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
	m_input_field->setText("sry dawg no autocompletion yet");
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
