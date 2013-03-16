/**
 * Class:	InterfaceManager
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef Canvas_H
#define Canvas_H

#include <map>
#include <vector>
#include "FastDelegate.h"
#include "JDHUtility/GlutInclude.h"
#include "InterfaceElement.h"
#include "JDHUtility/Point2f.h"

namespace MultiTouchEvents { class FingerEventArgs; };
namespace WavesVisuals { class Visual; }

using namespace fastdelegate;
using namespace MultiTouchEvents;
using namespace WavesVisuals;

namespace Waves
{
	class Finger;
	class FingerHeldCursor;
	class PopupMenu;
	class RadarColumn;
	class VisualWavesControl;
	class WavesControl;

	class Canvas : 
		public InterfaceElement
	{
	public:
		Canvas(const Point2f &position, float width, float height);
		~Canvas(void);

		bool contains(const FingerEventArgs &e) const;
		void fingerAdded(const FingerEventArgs &e);
		void fingerUpdated(const FingerEventArgs &e);
		void fingerRemoved(const FingerEventArgs &e);
		RadarColumn *getRadar(void);
		void idle(void);
		void render(void);
		void renderVisuals(bool mainScreen);
		void save(void);
		void update(void);
	private:
		static const float HELD_THRESHOLD;
		static const float HELD_CURSOR_SIZE;
		static const unsigned int HELD_CURSOR_PERIOD = 750;
		static const unsigned int INITIAL_LENGTH = 4;		
		static const float INITIAL_SCALE;
		static const unsigned int MAX_FINGERS = 2;
		static const unsigned int MENU_DELAY_MS = 200;
		static const float MENU_DIM;
		static const float MAX_SCALE;
		static const float OPACITY;
		static const float OUTER_REGION;
		static const float POPUP_SIZE;
		static const float RADAR_HEAD_SIZE;

		float canvasTransform[16];
		std::map<int, Finger *> fingers;
		FingerHeldCursor *fingerHeldCursor;
		PopupMenu *menu;
		RadarColumn *radar;
		float scale;

		void checkForMenuGesture(void);
		void constrainTranslate(void);
		void control_Closed(VisualWavesControl *control);
		void control_FingerLifted(WavesControl *control);
		void control_HeightChanged(void);
		void control_Moved(WavesControl* control);
		void control_Saved(VisualWavesControl *control);
		void fingerHeldCursor_FingerHeld(FingerHeldCursor *fingerHeld);
		void initInterfaceElements(void);
		void popupMenu_Exited(void);
		void popupMenu_SavedControlSelected(VisualWavesControl *control);
		void popupMenu_VisualSelected(Visual *visual);
		void renderInterfaceElements(void);
		void processSingleFinger(int anchorId, float dx, float dy);
		void processDoubleFinger(int actuatorId, float dx, float dy);
		void renderBackgroundMask(void);
		void setTransformToIdentity(void);
		void scaleCanvas(float s, Point2f centre);
		void translateCanvas(float tx, float ty);
		void wavesControl_Dragged(WavesControl *control, const Vector2f &v);
	};
}
#endif
