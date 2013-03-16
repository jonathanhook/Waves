/**
 * Class:	ExitButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef ExitButton_H
#define ExitButton_H

#include "Button.h"

namespace Waves
{
	class ExitButton :
		public Button
	{
	public:
		ExitButton(const Point2f &position, float width, float height);
		~ExitButton(void);

		void render(void);

	private:
		bool exitDirty;
		unsigned int exitDisplayList;
	};
}
#endif
