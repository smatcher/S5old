#ifndef SOUNDEMITTER_H
#define SOUNDEMITTER_H

#include "core/properties/iproperty.h"
#include "core/managers/managee.h"
#include "core/resources/sample.h"
#include "core/maths/vector3.h"
#include "core/managers/soundmanager.h"

#ifdef WITH_TOOLS
	#include "tools/widgets/soundemitterwidget.h"
#endif

#include <AL/al.h>

class SoundEmitter : public IProperty, public Managee<SoundManager>
{
    friend class SoundEmitterWidget;
public:
    enum Type {
        TYPE_GLOBAL,
        TYPE_DIRECTIONAL,
        TYPE_OMNIDIRECTIONAL
    };

    SoundEmitter(Sample& sample);

    void play();
    void pause();
    void stop();

    void setLoop(bool value);
    bool isLoop();

    void setGain(float gain);
    float getGain();

    void setPitch(float pitch);
    float getPitch();

    void setType(Type type);
    Type getType();

    void setPosition(Vector3d position);
    void setVelocity(Vector3d velocity);

	#ifdef WITH_TOOLS
		virtual PropertyWidget* getWidget();
	#endif
private:
    Sample   m_sample;
    ALuint  m_source;

    Type    m_type;
};

#endif // SOUNDEMITTER_H
