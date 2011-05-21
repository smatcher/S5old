#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "resource.h"

template <class Resource>
ResourceHandle<Resource>::ResourceHandle() : m_data(NULL) {}

template <class Resource>
ResourceHandle<Resource>::ResourceHandle(Resource& from) : m_data(&from)
{
	if(m_data != NULL)
		m_data->incRef();
}

template <class Resource>
ResourceHandle<Resource>::ResourceHandle(const ResourceHandle<Resource>& copy) : m_data(copy.m_data)
{
	if(m_data != NULL)
		m_data->incRef();
}

template <class Resource>
ResourceHandle<Resource>::~ResourceHandle()
{
	if(m_data != NULL)
		m_data->decRef();
}

template <class Resource>
bool ResourceHandle<Resource>::isValid()
{
	return m_data != NULL;
}

template <class Resource>
Resource* ResourceHandle<Resource>::operator->()
{
	return m_data;
}

template <class Resource>
Resource* ResourceHandle<Resource>::operator*()
{
	return m_data;
}

template <class Resource>
ResourceHandle<Resource>& ResourceHandle<Resource>::operator=(const ResourceHandle<Resource>& handle)
{
	if(m_data != NULL)
		m_data->decRef();

	m_data = handle.m_data;

	if(m_data != NULL)
		m_data->incRef();

	return (*this);
}

#endif // RESOURCE_HPP
