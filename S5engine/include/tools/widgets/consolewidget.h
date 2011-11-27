#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include "core/log/log.h"

#include <QFrame>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QIcon>

class ConsoleItemModel;

class ConsoleWidget : public QFrame
{
	Q_OBJECT

public:
	ConsoleWidget();
	~ConsoleWidget();

	void historyNext();
	void historyPrevious();
	void autocomplete();
	void echo(QString msg);
	void log(Log::LogItem& log);

public slots:
	virtual void newCommand();

private:
	static ConsoleWidget* m_instance;

	class ConsoleInputField : public QLineEdit
	{
	public:
		ConsoleInputField() : QLineEdit() {}
		virtual ~ConsoleInputField() {}

	protected:
		virtual void keyPressEvent(QKeyEvent * event);
		virtual bool event(QEvent * event);
	};

	QStringList m_history;
	int m_historyId;
	QListView* m_content_field;
	ConsoleItemModel* m_content_model;
	ConsoleInputField* m_input_field;
	QVBoxLayout* m_layout;

	QIcon m_info;
	QIcon m_warn;
	QIcon m_error;
	QIcon m_debug;
	QIcon m_ok;
	QIcon m_nok;
	QIcon m_input;
};

#endif // CONSOLEWIDGET_H
