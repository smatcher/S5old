#ifndef SINGLETON_H
#define SINGLETON_H

/*
  Implémente un phoenix singleton

  - Destruction automatique (relache les ressources)
  - Possibilité de faire revivre un singleton détruit (assure un service constant)
*/

template <class T>
class Singleton
{
public:

	static T& getInstance() // unique point of access
	{
		if (!instance)
		{
			if (flagDestroyed)
				onDeadReference();
			else
				create();
		}

		return *instance;
	}

	virtual ~Singleton()
	{
		instance = 0;
		flagDestroyed = true;
	}

private:

	static void create()
	{
		static T localInstance;
		instance = &localInstance;
	}

	static void onDeadReference()
	{
		create();
		// now instance points to the "ashes" of the singleton
		// create a new singleton at that address
		new(instance) T;
		atexit(kill);
		flagDestroyed = false;
	}

	static void kill(void)
	{
		delete instance;
		//instance->~T();
	}

	Singleton() {}
	static T* instance;
	static bool flagDestroyed;
};

template <class T>
T* Singleton<T>::instance=0;

template <class T>
bool Singleton<T>::flagDestroyed=false;

#endif // SINGLETON_H
