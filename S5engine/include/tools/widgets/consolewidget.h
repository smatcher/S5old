#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QFrame>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QIcon>

class ConsoleWidget : public QFrame
{
	Q_OBJECT

public:
	ConsoleWidget();
	~ConsoleWidget();

	static void MsgHandler(QtMsgType type, const char *msg);

	void historyNext();
	void historyPrevious();
	void autocomplete();

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
	QListWidget* m_content_field;
	ConsoleInputField* m_input_field;
	QVBoxLayout* m_layout;

	QIcon m_info;
	QIcon m_warn;
	QIcon m_error;
	QIcon m_ok;
	QIcon m_nok;
};

#endif // CONSOLEWIDGET_H
