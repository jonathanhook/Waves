/**
 * Class:	PreviewMonitor
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef PreviewMonitor_H
#define PreviewMonitor_H

#include "InterfaceElement.h"

namespace WavesVisuals { class Visual; }

using namespace WavesVisuals;

namespace Waves
{
	class PreviewMonitor : 
		public InterfaceElement
	{
	public:
		PreviewMonitor(const Point2f &position, float width, float height, Visual *visual);
		~PreviewMonitor(void);

		bool contains(const FingerEventArgs &e) const;
		void fingerAdded(const FingerEventArgs &e);
		void fingerUpdated(const FingerEventArgs &e);
		void fingerRemoved(const FingerEventArgs &e);
		void render(void);

	private:
		Visual *visual;
	};
}
#endif

