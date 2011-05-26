#ifndef SOUNDEMITTERWIDGET_H
#define SOUNDEMITTERWIDGET_H

#include "debug/widgets/propertywidget.h"
#include <QLineEdit>

class SoundEmitter;

class SoundEmitterWidget : public PropertyWidget
{
public :
        SoundEmitterWidget(SoundEmitter* property);
        virtual ~SoundEmitterWidget();

        virtual void updateData();

private :
        QLineEdit *m_sample, *m_type;
};

#endif // SOUNDEMITTERWIDGET_H
