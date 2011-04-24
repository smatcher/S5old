#ifndef SAMPLEFACTORY_H
#define SAMPLEFACTORY_H

#include "core/resources/iresourcefactory.h"
#include "core/resources/sample.h"

class SampleFactory : public IResourceFactory
{
public:
        virtual void load(ResourceData* resource);

protected:
        virtual void parseFile(const QString& path, QList<ResourceData*>& content);

private:
};

#endif // SAMPLEFACTORY_H

