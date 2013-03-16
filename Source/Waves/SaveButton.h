/**
 * Class:	SaveButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef SaveButton_H
#define SaveButton_H

#include "Button.h"

namespace Waves
{
	class SaveButton :
		public Button
	{
	public:
		SaveButton(const Point2f &position, float width, float height);
		~SaveButton(void);

		void render(void);

	private:
		bool saveButtonDirty;
		unsigned int saveButtonDisplayList;
	};
}
#endif

