#ifndef IRESOURCE_H
#define IRESOURCE_H

#include <QString>
#include "core/resources/iresourcefactory.h"

#include "core/log/log.h"

#ifdef WITH_TOOLS
	#include "tools/widgets/resourcewidget.h"
#endif

template <class Resource>
class ResourceHandle
{
public:
	ResourceHandle();
	ResourceHandle(Resource& from);
	ResourceHandle(const ResourceHandle<Resource>& copy);
	virtual ~ResourceHandle();

	bool isValid();

	Resource* operator->();
	Resource* operator*();
	ResourceHandle<Resource>& operator=(const ResourceHandle<Resource>& handle);

protected:
	Resource* m_data;
};

class ResourceData
{

#ifdef WITH_TOOLS
	friend class ResourceWidget;
#endif

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
		m_ref(0)
 #ifdef WITH_TOOLS
		, m_widget(NULL)
 #endif
		{}

	// Getters
	const QString& name() {return m_name;}
	const QString& path() {return m_path;}
	State state()         {return m_state;}

	virtual void load()   {m_factory->load(this);}
	bool isLoaded() {return m_state == STATE_LOADED;}

protected:
	IResourceFactory* m_factory;
	QString m_name;
	QString m_path;
	State   m_state;
	int     m_ref;

	// Reference counting
	void incRef()  {m_ref++;}
	void decRef()  {m_ref--; if(m_ref < 0) { logWarn( m_name << "reference count is negative"); }}
	int refCount() {return m_ref;}

	virtual bool unload() = 0;

#ifdef WITH_TOOLS

	protected:
		ResourceWidget* m_widget;

	private:
		void widgetDestroyed()
		{
			m_widget = NULL;
		}

	public:
		virtual ResourceWidget* getWidget()
		{
			if(!m_widget)
				m_widget = new ResourceWidget(*this);

			return m_widget;
		}
#endif
};

#include "resource.hpp"

#endif // IRESOURCE_H
