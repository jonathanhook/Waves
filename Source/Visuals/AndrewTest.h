/**
 * Class:	AndrewTest
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef AndrewTest_H
#define AndrewTest_H

#include "Visuals/Visual.h"

namespace WavesVisuals
{
	class AndrewTest :
		public Visual
	{
	public:
		AndrewTest(void);
		~AndrewTest(void);

		void draw(void);

	private:
		static const std::string RADIUS_NAME;
	};
}
#endif

