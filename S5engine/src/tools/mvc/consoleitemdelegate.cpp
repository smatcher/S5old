#include "tools/mvc/consoleitemdelegate.h"
#include "tools/mvc/consoleitemmodel.h"

#include <QTextLayout>
#include <QAbstractItemView>
#include <QPainter>
#include <QDir>

namespace {
const int ELLIPSIS_GRADIENT_WIDTH = 16;
}

class Positions
{
public:
	  Positions(const QStyleOptionViewItemV4 &options, ConsoleItemModel *model) :
			m_totalWidth(options.rect.width()),
			m_maxTopicLength(model->sizeOfTopic(options.font)),
			m_realTopicLength(m_maxTopicLength),
			m_top(options.rect.top()),
			m_bottom(options.rect.bottom())
	  {
			int flexibleArea = topicAreaLeft() - textAreaLeft() - ITEM_SPACING;
			if (m_maxTopicLength > flexibleArea / 2)
				 m_realTopicLength = flexibleArea / 2;
			m_fontHeight = QFontMetrics(options.font).height();
	  }

	  int top() const { return m_top + ITEM_MARGIN; }
	  int left() const { return ITEM_MARGIN; }
	  int right() const { return m_totalWidth - ITEM_MARGIN; }
	  int bottom() const { return m_bottom; }
	  int firstLineHeight() const { return m_fontHeight + 1; }
	  int minimumHeight() const { return iconHeight() + 2 * ITEM_MARGIN; }
	  int totalWidth() const { return m_totalWidth - 2*ITEM_MARGIN; }

	  int iconLeft() const { return left(); }
	  int iconWidth() const { return TASK_ICON_SIZE; }
	  int iconHeight() const { return TASK_ICON_SIZE; }
	  int iconRight() const { return iconLeft() + iconWidth(); }
	  QRect icon() const { return QRect(iconLeft(), top(), iconWidth(), iconHeight()); }

	  int textAreaLeft() const { return iconRight() + ITEM_SPACING; }
	  int textAreaWidth() const { return textAreaRight() - textAreaLeft(); }
	  int textAreaRight() const { return topicAreaLeft() - ITEM_SPACING; }
	  QRect textArea() const { return QRect(textAreaLeft(), top(), textAreaWidth(), firstLineHeight()); }

	  int topicAreaLeft() const { return topicAreaRight() - topicAreaWidth(); }
	  int topicAreaWidth() const { return m_realTopicLength; }
	  int topicAreaRight() const { return right() - ITEM_SPACING; }
	  QRect topicArea() const { return QRect(topicAreaLeft(), top(), topicAreaWidth(), firstLineHeight()); }

 private:
	  int m_totalWidth;
	  int m_maxTopicLength;
	  int m_realTopicLength;
	  int m_top;
	  int m_bottom;
	  int m_fontHeight;

	  static const int TASK_ICON_SIZE = 16;
	  static const int ITEM_MARGIN = 2;
	  static const int ITEM_SPACING = 2 * ITEM_MARGIN;
};

ConsoleItemDelegate::ConsoleItemDelegate(QObject* parent) : QStyledItemDelegate(parent)
{
}

ConsoleItemDelegate::~ConsoleItemDelegate()
{
}

QSize ConsoleItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);

	const QAbstractItemView * view = qobject_cast<const QAbstractItemView *>(opt.widget);
	const bool selected = (view->selectionModel()->currentIndex() == index);
	QSize s;
	s.setWidth(option.rect.width());

	QFontMetrics fm(option.font);
	int fontHeight = fm.height();
	int fontLeading = fm.leading();

	ConsoleItemModel *model = static_cast<ConsoleItemModel *>(view->model());
	Positions positions(option, model);

	 QString message = index.data(ConsoleItemModel::Message).toString();
	 // Layout the description
	 int leading = fontLeading;
	 int height = 0;
	 message.replace('\n', QChar::LineSeparator);
	 QTextLayout tl(message);
	 tl.beginLayout();
	 while (true) {
		  QTextLine line = tl.createLine();
		  if (!line.isValid())
				break;
		  line.setLineWidth(positions.textAreaWidth());
		  height += leading;
		  line.setPosition(QPoint(0, height));
		  height += static_cast<int>(line.height());
	 }
	 tl.endLayout();

	 if(selected && !index.data(ConsoleItemModel::File).toString().isEmpty())
	 {
		 height += leading + fontHeight;
	 }

	 s.setHeight(height + 3);

	if (s.height() < positions.minimumHeight())
		 s.setHeight(positions.minimumHeight());

	if (!selected) {
		 m_cachedFont = option.font;
	}

	return s;
}

void ConsoleItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);
	painter->save();
	QFontMetrics fm(opt.font);
	QColor backgroundColor;
	QColor textColor;
	const QAbstractItemView * view = qobject_cast<const QAbstractItemView *>(opt.widget);
	bool selected = view->selectionModel()->currentIndex() == index;
	if (selected) {
	  painter->setBrush(opt.palette.highlight().color());
	  backgroundColor = opt.palette.highlight().color();
	} else {
	  painter->setBrush(opt.palette.background().color());
	  backgroundColor = opt.palette.background().color();
	}
	painter->setPen(Qt::NoPen);
	painter->drawRect(opt.rect);
	// Set Text Color
	if (selected)
	  textColor = opt.palette.highlightedText().color();
	else
	  textColor = opt.palette.text().color();
	painter->setPen(textColor);
	ConsoleItemModel *model = static_cast<ConsoleItemModel *>(view->model());
	Positions positions(opt, model);
	// Paint TaskIconArea:
	QIcon icon = index.data(ConsoleItemModel::Icon).value<QIcon>();
	painter->drawPixmap(positions.left(), positions.top(),
							icon.pixmap(positions.iconWidth(), positions.iconHeight()));
	// Paint TextArea:
	QString description = index.data(ConsoleItemModel::Message).toString();
	QString topic = index.data(ConsoleItemModel::Topic).toString();
	// Layout the description
	int leading = fm.leading();
	int height = 0;
	description.replace('\n', QChar::LineSeparator);
	QTextLayout tl(description);
	tl.beginLayout();
	while (true) {
		QTextLine line = tl.createLine();
		if (!line.isValid())
			 break;
		if(topic.isEmpty())
			line.setLineWidth(positions.totalWidth());
		else
			line.setLineWidth(positions.textAreaWidth());

		height += leading;
		line.setPosition(QPoint(0, height));
		height += static_cast<int>(line.height());
	}
	tl.endLayout();
	tl.draw(painter, QPoint(positions.textAreaLeft(), positions.top()));

	if(selected)
	{
		QColor mix;
		mix.setRgb( static_cast<int>(0.7 * textColor.red()   + 0.3 * backgroundColor.red()),
					 static_cast<int>(0.7 * textColor.green() + 0.3 * backgroundColor.green()),
					 static_cast<int>(0.7 * textColor.blue()  + 0.3 * backgroundColor.blue()));
		painter->setPen(mix);
		QString directory = QDir::toNativeSeparators(index.data(ConsoleItemModel::File).toString());
		if(!directory.isEmpty())
		{
			directory.append(":"+QString().setNum(index.data(ConsoleItemModel::Line).toInt()));
			directory.prepend(index.data(ConsoleItemModel::Function).toString() + "()  ");
			int secondBaseLine = positions.top() + fm.ascent() + height + leading;
			painter->drawText(positions.textAreaLeft(), secondBaseLine, directory);
		}
	}

	painter->setPen(textColor);
	// Paint TopicArea
	const int realTopicWidth = fm.width(topic);
	painter->setClipRect(positions.topicArea());
	painter->drawText(qMin(positions.topicAreaLeft(), positions.topicAreaRight() - realTopicWidth),
						 positions.top() + fm.ascent(), topic);
	if (realTopicWidth > positions.topicAreaWidth()) {
	  // draw a gradient to mask the text
	  int gradientStart = positions.topicAreaLeft() - 1;
	  QLinearGradient lg(gradientStart + ELLIPSIS_GRADIENT_WIDTH, 0, gradientStart, 0);
	  lg.setColorAt(0, Qt::transparent);
	  lg.setColorAt(1, backgroundColor);
	  painter->fillRect(gradientStart, positions.top(), ELLIPSIS_GRADIENT_WIDTH, positions.firstLineHeight(), lg);
	}
	// Separator lines
	painter->setPen(QColor::fromRgb(150,150,150));
	painter->drawLine(0, opt.rect.bottom(), opt.rect.right(), opt.rect.bottom());
	painter->restore();
}

void ConsoleItemDelegate::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
	emit sizeHintChanged(current);
	emit sizeHintChanged(previous);
}
