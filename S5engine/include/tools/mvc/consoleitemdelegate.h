#ifndef CONSOLEITEMDELEGATE_H
#define CONSOLEITEMDELEGATE_H

#include <QStyledItemDelegate>

class ConsoleItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT

	public:

		ConsoleItemDelegate(QObject* parent = 0);
		virtual ~ConsoleItemDelegate();

		virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
		virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	public slots:
		void currentChanged(const QModelIndex &current, const QModelIndex &previous);

	private:
		mutable QFont m_cachedFont;
};

#endif // CONSOLEITEMDELEGATE_H
