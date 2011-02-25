/**
  file :
	parenting.h

  date :
	Feb 19 2011

  author :
	Benjamin Hautbois <bhautboi@gmail.com>

  description :
	This file provides the following classes : ParentOf and ChildOf
	These classes provide means for generic handling of named parenting.

	Named parenting means that every child is identified by a unique name.

  */

#ifndef PARENTING_H
#define PARENTING_H

#include <QMap>
#include <QString>

/**

		/!\ TODO : Utiliser QHash au lieu de QMap (plus rapide et on a pas besoin du multimap)
		btw : eviter de rechercher par nom à chaque fois que le fils demande au pere de faire une operation

		/!\ TODO : Tests unitaires (beaucoup)

  **/

/**

  class :
	ChildOf

  */
template <class Parent>
class ChildOf
{
public :
	/// Constructor, if parent is different from NULL, the constructor will ask the parent to make the link.
	ChildOf(const QString& name, Parent* parent = NULL);
	/// Destructor
	virtual ~ChildOf();

	/// Name getter
	const QString& getName();
	/// Name setter
	void setName(const QString& name);
	/// Parent getter
	Parent* parent();
	/// Index from parent
	int childNumber();
	/// Unlink from the parent, the object is not destroyed
	/// (justForget won't ask the parent to unlink, if you don't know what you are doing just forget this argument)
	void unlinkFromParent(bool justForget = false);
	/// Set the parent, don't use it directly, it won't tell the parent to link, link from the parent instead
	void setParent(void* parent);

private :
	/// Reference to the parent, NULL if no parent
	Parent* m_parent;
	/// Name of the child, must be different from its brothers's name.
	QString m_name;
};

/**

  class :
	ParentOf

  */
template <class Child>
class ParentOf
{
public :
	/// Constructor
	ParentOf();
	/// Destructor
	~ParentOf();

	/// Get child based on his name
	Child* child(const QString& childName) const;
	/// Get the n'th child
	Child* child(int nb) const;
	/// Get the number of chidren
	int childCount() const;
	/// Link
	void link(Child* son);
	void link(ChildOf< ParentOf<Child> >* son);
	/// Unlink from child, the child is not destroyed, returns a pointer to the child
	Child* unlink(const QString& childName);
	/// Unlink from all childs, children are not destroyed
	void unlinkAll();

	typedef Child ChildClass;

private :
	QMap<QString, Child*> m_sons;
};

#include "parenting.hpp"

#endif // PARENTING_H
