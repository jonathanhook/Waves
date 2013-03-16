/**
 * Class:	SubtractButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef SubtractButton_H
#define SubtractButton_H

#include "Button.h"

namespace JDHUtility { class Point2f; }
using namespace JDHUtility;

namespace Waves
{
	class SubtractButton :
		public Button
	{
	public:
		SubtractButton(const Point2f &position, float width, float height);
		~SubtractButton(void);

		void render(void);

	private:
		bool subtractButtonDirty;
		unsigned int subtractButtonDisplayList;
	};
}
#endif

