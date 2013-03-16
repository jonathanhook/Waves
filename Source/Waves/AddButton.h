/**
 * Class:	AddButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef AddButton_H
#define AddButton_H

#include "Button.h"

namespace JDHUtility { class Point2f; }
using namespace JDHUtility;

namespace Waves
{
	class AddButton :
		public Button
	{
	public:
		AddButton(const Point2f &position, float width, float height);
		~AddButton(void);

		void render(void);

	private:
		bool addButtonDirty;
		unsigned int addButtonDisplayList;
	};
}
#endif

