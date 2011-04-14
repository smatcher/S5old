#ifndef IRESOURCE_H
#define IRESOURCE_H

#include <QString>
#include <QDebug>

class IResourceFactory;

template <class Resource>
class ResourceHandle
{
public:
	ResourceHandle();
	ResourceHandle(Resource& from);
	ResourceHandle(const ResourceHandle<Resource>& copy);
	virtual ~ResourceHandle();

	Resource* operator&();
	ResourceHandle<Resource>& operator=(const ResourceHandle<Resource>& handle);

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

	ResourceData(const QString& name, const QString& path, IResourceFactory* factory) :
		m_factory(factory),
		m_name(name),
		m_path(path),
		m_state(STATE_UNLOADED),
		m_ref(0) {}

	// Reference counting
	void incRef()  {m_ref++;}
	void decRef()  {m_ref--; if(m_ref < 0) { qDebug() << "Warning : " << m_name << " reference count is negative";}}
	int refCount() {return m_ref;}

	// Getters
	const QString& name() {return m_name;}
	const QString& path() {return m_path;}
	State state()         {return m_state;}
	const IResourceFactory* factory() {return m_factory;}

	virtual bool unload() = 0;

protected:
	IResourceFactory* m_factory;
	QString m_name;
	QString m_path;
	State   m_state;
	int     m_ref;
};

#include "resource.hpp"

#endif // IRESOURCE_H
