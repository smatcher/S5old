#include "core/properties/soundemitter.h"
#include "tools/widgets/soundemitterwidget.h"

SoundEmitter::SoundEmitter(Sample &sample) :
    m_sample(sample), IProperty("Sound Emitter")
{
    m_type = TYPE_OMNIDIRECTIONAL;

    alGenSources(1, &m_source);
    ((SampleData*)&m_sample)->bind(m_source);
}

void SoundEmitter::setType(SoundEmitter::Type type)
{
    m_type = type;
}

SoundEmitter::Type SoundEmitter::getType()
{
    return m_type;
}


void SoundEmitter::play()
{
    alSourcePlay(m_source);
}

void SoundEmitter::pause()
{
    alSourcePause(m_source);
}

void SoundEmitter::stop()
{
    alSourceStop(m_source);
}

void SoundEmitter::setLoop(bool value)
{
    alSourcei(m_source, AL_LOOPING, value?AL_TRUE:AL_FALSE );
}

bool SoundEmitter::isLoop()
{
    ALint loop;
    alGetSourcei(m_source, AL_LOOPING, &loop);
    return (loop==AL_TRUE);
}

void SoundEmitter::setGain(float gain)
{
    alSourcef(m_source, AL_GAIN, gain);
}

float SoundEmitter::getGain()
{
    ALfloat gain;
    alGetSourcef(m_source, AL_GAIN, &gain);
    return (float)gain;
}

void SoundEmitter::setPitch(float pitch)
{
    alSourcef(m_source, AL_PITCH, pitch);
}

float SoundEmitter::getPitch()
{
    ALfloat pitch;
    alGetSourcef(m_source, AL_PITCH, &pitch);
    return (float)pitch;
}

void SoundEmitter::setPosition(Vector3d position)
{
    ALfloat positionv[] = {(float)position.coords[0],(float)position.coords[1],(float)position.coords[2]};
    alSourcefv(m_source, AL_POSITION, positionv);
}

void SoundEmitter::setVelocity(Vector3d velocity)
{
    ALfloat velocityv[] = {(float)velocity.coords[0],(float)velocity.coords[1],(float)velocity.coords[2]};
    alSourcefv(m_source, AL_VELOCITY, velocityv);
}

#ifdef WITH_TOOLS

PropertyWidget* SoundEmitter::getWidget()
{
	if(m_widget == 0)
		m_widget = new SoundEmitterWidget(this);

	return m_widget;
}

#endif
