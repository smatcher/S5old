#ifndef CUSTOMEVENTS_H
#define CUSTOMEVENTS_H

#include <QEvent>

enum CustomEvent
{
	UPDATED
};

template <CustomEvent type>
class Event : public QEvent
{
public :
	Event() : QEvent(buildType()) {}

	static QEvent::Type type()
	{
		static QEvent::Type _type = static_cast<QEvent::Type>(registerEventType());
		return _type;
	}

	QEvent::Type buildType()
	{
		return type();
	}
};

typedef Event<UPDATED> UPDATED_EVENT;

#endif // CUSTOMEVENTS_H
