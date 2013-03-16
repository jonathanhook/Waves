/**
 * Class:	Singleton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef Singleton_H
#define Singleton_H

namespace JDHUtility
{
	class Singleton
	{
	public:
		static void clearInstance(void);
		static Singleton *getInstance(void);

	protected:
		Singleton(void);
		~Singleton(void);

		static Singleton *instance;
	};
}
#endif

