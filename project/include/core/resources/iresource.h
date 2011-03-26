#ifndef IRESOURCE_H
#define IRESOURCE_H

#include <QString>

template <class ResourceManager>
class IResource
{
public:
	typedef ResourceManager Manager;

	enum State
	{
		STATE_INITIALIZED,
		STATE_UNLOADED,
		STATE_LOADED
	};

protected:
	QString m_path;

};

#endif // IRESOURCE_H
