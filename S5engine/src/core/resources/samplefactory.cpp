#include "core/resources/samplefactory.h"
#include "core/resources/managers.h"

#include <AL/al.h>
#include <AL/alut.h>
#include <QDir>

#include "debug/log/log.h"

void SampleFactory::load(ResourceData* resource)
{
		SampleData* sample_data = static_cast<SampleData*>(resource);

		/* Sound related data */
		ALenum format;
		ALsizei size;
		ALvoid* data;
		ALsizei freq;
		ALboolean loop;

		if(resource->state() == SampleData::STATE_UNLOADED)
		{
			/* Allocating the new buffer */
			alGenBuffers(1,&(sample_data->m_buffer));

			/* Load the file to "data" */
			alutLoadWAVFile((ALbyte*)sample_data->path().toLocal8Bit().data(), &format, &data, &size, &freq, &loop);

			/*TODO: correct error handling?*/
			if(alGetError() == AL_NO_ERROR)
			{
				logInfo( "Sound loaded "<< resource->name() );

				/* Copy data to the new buffer */
				alBufferData(sample_data->m_buffer, format, data, size, freq);

				/* Free data */
				alutUnloadWAV(format, data, size, freq);
			}
			else
				logError( "OpenAL failed to load " << resource->name() << "\n" << alGetError() );
		}
}



void SampleFactory::parseFile(const QString &path, QList<ResourceData *> &content, const QHash<QString,QString>&)
{
		QDir dir(path);
		QString name = dir.dirName();
		dir.cdUp();
		if(dir.exists())
		{
				debug( "RESOURCE PARSING" ,"Sample found " << name );
				SampleData* sample = new SampleData(name,path,this);
				content.push_back(sample);
		}
		else
		{
				debug( "RESOURCE PARSING" , path << " : " << dir << " does not exist");
		}
}

