/**
 * Class:	FFTButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef FFTButton_H
#define FFTButton_H

#include "Button.h"

namespace Waves
{
	class FFTButton :
		public Button
	{
	public:
		FFTButton(const Point2f &position, float width, float height);
		~FFTButton(void);

		void render(void);

	private:
		bool fftButtonDirty;
		unsigned int fftButtonDisplayList;
	};
}
#endif

