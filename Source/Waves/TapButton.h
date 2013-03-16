/**
 * Class:	TapButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef TapButton_H
#define TapButton_H

#include "Button.h"

namespace JDHUtility { class Point2f; }
using namespace JDHUtility;

namespace Waves
{
	class TapButton :
		public Button
	{
	public:
		TapButton(const Point2f &position, float width, float height);
		~TapButton(void);

		void render(void);
		void reset(void);
		void setBpm(float bpm);

	private:
		bool tapButtonDirty;
		unsigned int tapButtonDisplayList;
		std::string tapText;
	};
}
#endif

