#include "core/properties/soundemitter.h"
#include "debug/widgets/soundemitterwidget.h"

SoundEmitter::SoundEmitter(Sample &sample) :
    m_sample(sample), IProperty("Sound Emitter")
{
    m_type = TYPE_OMNIDIRECTIONAL;

    alGenSources(1, &m_source);
    ((SampleData*)&m_sample)->bind(m_source);
}

PropertyWidget* SoundEmitter::getWidget()
{
        if(m_widget == NULL)
                m_widget = new SoundEmitterWidget(this);

        return m_widget;
}
