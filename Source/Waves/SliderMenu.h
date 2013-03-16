/* Waves -- SliderMenu
 * Author: Jonathan David Hook (j.d.hook@ncl.ac.uk)
 * Date: 25/11/2009
 */
#ifndef SliderMenu_H
#define SliderMenu_H

#include "JDHUtility/Point2f.h"
#include "InterfaceElement.h"

namespace Waves
{
	class SliderMenu :
		public InterfaceElement
	{
	public:
		SliderMenu(const Point2f &position);
		~SliderMenu(void);

		void render(void);
	private:
		static float DEFAULT_WIDTH;
		static float DEFAULT_HEIGHT;
		static int MENU_ITEM_COUNT;
		static float PINCH_FACTOR;
		static int SEGMENTS;

		float xOffset;
		float index;
		vector<WaveVisual*> visuals;

		float getYPos(float mu, bool top);
	};
}
#endif
