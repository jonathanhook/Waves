/**
 * Class:	RadarHead
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef RadarHead_H
#define RadarHead_H

#include <string>
#include "JDHUtility/GLTexture.h"
#include "JDHUtility/GlutInclude.h"

namespace Waves
{
	class RadarHead :
		public InterfaceElement
	{
	public:
		RadarHead(const Point2f &position, float width, float height);
		~RadarHead(void);

		bool contains(const FingerEventArgs &e) const;
		void fingerAdded(const FingerEventArgs &e);
		void fingerUpdated(const FingerEventArgs &e);
		void fingerRemoved(const FingerEventArgs &e);
		void render(void);
	private:
		static const float MAX_OPACITY;
		static const string TEXTURE_URI;		
	};
}
#endif
