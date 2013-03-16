/**
 * Class:	RadarColumn
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef RadarColumn_H
#define RadarColumn_H

#include <string>
#include <vector>
#include "InterfaceElement.h"
#include "JDHUtility/Point2f.h"
#include "RadarElement.h"
#include "RadarSegment.h"

namespace JDHUtility { class GLTexture; }
using namespace JDHUtility;

namespace Waves
{
	class RadarColumn : 
		public RadarElement
	{
	public:
		RadarColumn(const Point2f &position, float width, float height);
		~RadarColumn(void);

		void addControl(WavesControl *control); 
		void fingerAdded(const FingerEventArgs &e);
		void fingerUpdated(const FingerEventArgs &e);
		void fingerRemoved(const FingerEventArgs &e);
		void removeControl(WavesControl *control);
		void render(void);
		void sortSegments(void);
		void updateSegments(WavesControl *control);
	private:
		static float HEAD_WIDTH;
		static float HEAD_OPACITY;
		static string LINE_TEXTURE;
		static float NUM_DASHES;
		static float RANGE;

		float lastX;
		float playheadOffset;
		std::vector<WavesControl *> controls;

		bool contains(Point2f p);
		bool controlInRange(WavesControl *control);
		void renderBackgroundMask(void);
	};
}
#endif
