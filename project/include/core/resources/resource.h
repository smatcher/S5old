#ifndef IRESOURCE_H
#define IRESOURCE_H

#include <QString>

class IResourceFactory;

template <class Resource>
class ResourceHandle
{
public:
	ResourceHandle(const Resource& from);
	ResourceHandle(const ResourceHandle& copy);
	virtual ~ResourceHandle();

	Resource* operator&();
	void operator=();

private:
	Resource* m_data;
};

class ResourceData
{
public:

	enum State
	{
		STATE_UNLOADED,
		STATE_LOADED
	};

	ResourceData(const QString& name, const QString& path, IResourceFactory* factory);

	void incRef();
	void decRef();
	int refCount();

	const QString& name();
	const QString& path();
	const State state();
	const IResourceFactory* factory();

	void unload();

protected:
	IResourceFactory* m_factory;
	QString m_name;
	QString m_path;
	State   m_state;
	int     m_ref;
};

#include "resource.hpp"

#endif // IRESOURCE_H
