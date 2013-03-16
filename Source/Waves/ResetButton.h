/**
 * Class:	ResetButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef ResetButton_H
#define ResetButton_H

#include "Button.h"

namespace Waves
{
	class ResetButton :
		public Button
	{
	public:
		ResetButton(const Point2f &position, float width, float height);
		~ResetButton(void);

		void render(void);

	private:
		bool resetButtonDirty;
		unsigned int resetButtonDisplayList;
	};
}
#endif
