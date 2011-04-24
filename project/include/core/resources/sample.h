#ifndef SAMPLE_H
#define SAMPLE_H

#include "core/resources/resource.h"
#include <AL/al.h>

class SampleData : public ResourceData
{
    friend class SampleFactory;

private:
        ALuint m_buffer;
public:
        SampleData(const QString& name, const QString& path, IResourceFactory* factory) : ResourceData(name,path,factory) {}
        void bind(ALuint source)
        {
            alSourcei (source, AL_BUFFER, m_buffer);
        }

        bool unload()
        {
            alDeleteBuffers(1, &m_buffer);
            return true; // wtf??
        }
};

class Sample : public ResourceHandle<SampleData>
{
public:
        Sample() {}
        Sample(const Sample& from) : ResourceHandle<SampleData>(from) {}
        Sample(SampleData& from) : ResourceHandle<SampleData>(from) {}
        virtual ~Sample() {}
};

#endif // SAMPLE_H
