// This include should fail since parenting.h includes parenting.hpp
// However QtCreator needs this include in order to provide code assistance.
#include "parenting.h"

/*
 *  ParentOf
 */

template<class Child>
ParentOf<Child>::ParentOf()
{
}

template<class Child>
ParentOf<Child>::~ParentOf()
{
	// Free all sons still linked to the parent
	// The parent destroyer should have unlinked them.
	QMutableMapIterator<QString, Child*> i(m_sons);
	 while (i.hasNext()) {
		 i.next();
		 Child* save = i.value();
		 save->unlinkFromParent(true);	// Unlink the son
		 m_sons.remove(i.key());		// Forget the son
		 delete save;					// Destroy the son
	 }
}

template<class Child>
Child* ParentOf<Child>::child(const QString& childName) const
{
	return m_sons.value(childName,NULL);
}

template<class Child>
Child* ParentOf<Child>::child(int nb) const
{
	QList<QString> keys = m_sons.keys();

	return child(keys[nb]);
}

template<class Child>
int ParentOf<Child>::childCount() const
{
	return m_sons.count();
}

template<class Child>
void ParentOf<Child>::link(Child* son)
{
	#ifdef DEBUG
		assert(son != NULL);
	#endif

	if(son != NULL)
	{
		const QString& name = son->getName();

		if(!m_sons.contains(name))
		{
			m_sons.insert(name,son);
			son->setParent((typename Child::ParentPtrType)this);
			onLinked(son);
		}
		else
		{
			// TODO print a warning (there is already a son named like this)
		}
	}
	else
	{
		// TODO print a warning (trying to add NULL)
	}
}

template<class Child>
Child* ParentOf<Child>::unlink(const QString& childName)
{
	Child* ret = NULL;
	typename QMap<QString, Child*>::iterator i = m_sons.find(childName);
	if(i != m_sons.end() && i.key() == childName)
	{
		ret = i.value();
		ret->unlinkFromParent(true);
		m_sons.erase(i);
		onUnlinked(ret);
	}
	return ret;
}

template<class Child>
void ParentOf<Child>::unlinkAll()
{
	QMutableMapIterator<QString, Child*> i(m_sons);
	 while (i.hasNext()) {
		 i.next();
		 Child* save = i.value();
		 save->unlinkFromParent(true);	// Unlink the son
		 m_sons.remove(i.key());		// Forget the son
	 }
}

/*
 *  ChildOf
 */

template<class Parent>
ChildOf<Parent>::ChildOf(const QString& name, Parent* parent)
{
	m_parent = parent;
	m_name = name;

	if(parent != NULL)
		parent->link((typename Parent::ChildPtrType)this);
}

template<class Parent>
ChildOf<Parent>::~ChildOf()
{
	// Unlink from the parent to avoid dangling pointers
	unlinkFromParent();
}

template<class Parent>
const QString& ChildOf<Parent>::getName()
{
	return m_name;
}

template<class Parent>
void ChildOf<Parent>::setName(const QString& name)
{
	// Save parent
	Parent* parent = m_parent;
	// Unlink
	unlinkFromParent();
	// Change name
	m_name = name;
	// Relink
	parent->link(this);
}

template<class Parent>
Parent* ChildOf<Parent>::parent()
{
	return m_parent;
}

template<class Parent>
int ChildOf<Parent>::childNumber()
{
	int ret = 0;

	if(m_parent != NULL)
	{
		for(int i=0 ; i<m_parent->childCount() ; i++)
		{
			if(m_parent->child(i) == this)
			{
				ret = i;
				break;
			}
		}
	}

	return ret;
}

template<class Parent>
void ChildOf<Parent>::unlinkFromParent(bool justForgetParent)
{
	if(m_parent != NULL && !justForgetParent)
	{
		m_parent->unlink(m_name);
	}
	Parent* oldParent = m_parent;
	m_parent = NULL;
	onUnlinked(oldParent);
}

template<class Parent>
void ChildOf<Parent>::setParent(Parent* parent)
{
	m_parent = static_cast<Parent*>(parent);
	onLinked(m_parent);
}
