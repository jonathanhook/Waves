/**
 * Class:	Singleton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <stdio.h>
#include "JDHUtility/Singleton.h"

namespace JDHUtility
{
	/* STATIC VARS */
	Singleton *Singleton::instance = NULL;

	/* CONSTRUCTORS */
	Singleton::Singleton(void)
	{
	}

	Singleton::~Singleton(void)
	{
	}

	/* PUBLIC STATIC FUNCTIONS */
	void Singleton::clearInstance(void)
	{
		if(instance != NULL)
		{
			delete instance;
			instance = NULL;
		}
	}

	Singleton *Singleton::getInstance(void)
	{
		if(instance == NULL)
		{
			instance = new Singleton();
		}

		return instance;
	}
}
