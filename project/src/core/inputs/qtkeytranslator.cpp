#include "core/inputs/qtkeytranslator.h"
#include <QMetaEnum>

QtKeyTranslator::QtKeyTranslator()
{
	int index = staticQtMetaObject.indexOfEnumerator("Key");
	m_enum = staticQtMetaObject.enumerator( index);
}

int QtKeyTranslator::toValue(const QString& name)
{
	QString keyString = name;
	keyString = keyString.replace("KB_","Key_");;
	return m_enum.keyToValue(keyString.toLocal8Bit().data());
}

QString QtKeyTranslator::toName(int value)
{
	QString keyString = m_enum.valueToKey( value );
	return keyString.replace("Key_","KB_");
}
