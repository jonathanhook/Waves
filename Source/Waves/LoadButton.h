/**
 * Class:	LoadButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef LoadButton_H
#define LoadButton_H

#include "ToggleButton.h"

namespace Waves
{
	class LoadButton :
		public ToggleButton
	{
	public:
		LoadButton(const Point2f &position, float width, float height, bool toggled = false);
		~LoadButton(void);

		void render(void);

	private:
		bool loadButtonDirty;
		unsigned int loadButtonDisplayList;
	};
}
#endif