/**
 * Class:	InterfaceManager
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef InterfaceManager_H
#define InterfaceManager_H

#include <map>
#include <vector>
#include "Canvas.h"
#include "JDHUtility/GlutInclude.h"
#include "MultiTouchEvents/MultiTouchEventManager.h"
#include "JDHUtility/Vector2f.h"

namespace MultiTouchEvents { class FingerEventArgs; };

using namespace JDHUtility;
using namespace MultiTouchEvents;

namespace Waves
{
	class InterfaceElement;
	class SplinePreset;
	class WaveVisual;

	class InterfaceManager :
		public MultiTouchEventManager
	{
	public:
		static const unsigned int BEATS;
		static const float CAMERA_Z;
		static const float CAMERA_FOV;
		static const unsigned int TRACK_DIV;
		static const unsigned int VALUES_PER_BEAT;
		static const float VANISHING_POINT_Z;
		static const float VISUAL_Z;
		static const float WORLD_WIDTH;
		static const float WORLD_HEIGHT;

		static float getBeatLength(void);
		static float getTrackHeight(void);

		InterfaceManager(bool dispatchEvents);
		~InterfaceManager(void);

		std::vector<WaveVisual*> getVisuals() const;
		void renderInterface(void);
		void renderVisuals(bool mainScreen);
		void save(void);
		void update(void);

	private:
		std::map<unsigned int, Finger *> fingers;
		InterfaceElement *interfaceRoot;
		bool showFingers;

		void peek(const FingerEventArgs &e);
		void reportFPS(void);
	};
}
#endif
