/**
 * Class:	RollingAverage
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef RollingAverage_H
#define RollingAverage_H

#include <assert.h>
#include <math.h>
#include <queue>

namespace JDHUtility
{
	template <class T>
	class RollingAverage
	{
	public:
		RollingAverage(unsigned int bufferSize = 10);
		~RollingAverage(void);

		void addValue(T value);
		T getAverage(void) const;
		unsigned int getBufferSize(void) const;

	private:
		static const unsigned int DEFAULT_BUFFER_SIZE = 10;

		T *buffer;
		unsigned int bufferSize;
		unsigned int ptr;
		T total;
	};

	/* CONSTRUCTORS */
	template<class T>
	RollingAverage<T>::RollingAverage(unsigned int bufferSize)
	{
		this->bufferSize = bufferSize;
		buffer = new T[bufferSize];
		ptr = 0;
		total = 0.0f;

		for(unsigned int i = 0; i < bufferSize; i++)
		{
			buffer[i] = 0;
		}
	}

	template<class T>
	RollingAverage<T>::~RollingAverage(void)
	{
		delete [] buffer;
	}

	/* PUBLIC MEMBER FUNCTIONS */
	template<class T>
	void RollingAverage<T>::addValue(T value)
	{
		// BROKEN

		total -= buffer[++ptr % bufferSize];
		if(total != total)
		{
			total += buffer[ptr % bufferSize];
		}
		else
		{
			buffer[ptr % bufferSize] = value;
			total += value;
		}

		assert(total == total);
	}

	template<class T>
	T RollingAverage<T>::getAverage(void) const
	{
		if(ptr >= bufferSize)
		{
			return total / (T)bufferSize;
		}
		else if(ptr > 0)
		{
			return total / (T)ptr;
		}
		else
		{
			return total;
		}
	}

	template<class T>
	unsigned int RollingAverage<T>::getBufferSize(void) const
	{
		return bufferSize;
	}
}
#endif

