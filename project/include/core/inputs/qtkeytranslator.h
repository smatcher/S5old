#ifndef QTKEYTRANSLATOR_H
#define QTKEYTRANSLATOR_H

#include <QObject>
#include <QMetaEnum>

class QtKeyTranslator : public QObject
{
public:
	QtKeyTranslator();

	int toValue(const QString& name);
	QString toName(int value);

private:
	QMetaEnum m_enum;
};

#endif // QTKEYTRANSLATOR_H
