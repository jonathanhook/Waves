/**
 * Class:	BPMControl
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef BPMControl_H
#define BPMControl_H

#include "InterfaceElement.h"

class TapBpm;

namespace Waves
{
	class TapButton;
	class TextLabel;

	class BPMControl :
		public InterfaceElement
	{
	public:
		BPMControl(const Point2f position, float width, float height);
		~BPMControl(void);

		bool contains(const FingerEventArgs &e) const;
		void fingerAdded(const FingerEventArgs &e);
		void fingerUpdated(const FingerEventArgs &e);
		void fingerRemoved(const FingerEventArgs &e);
		void render(void);
		void update(void);

	private:
		static const float AS_WIDTH;
		static const float BPM_INCREMENT;
		static const unsigned int DOUBLE_RATE = 250;
		static const unsigned int RESET_TIME = 2000;
		static const float TAP_HEIGHT;
		static const float TAP_WIDTH;
		static const unsigned int UPDATE_RATE = 16;

		bool addHeld;
		int addHeldTime;
		TextLabel *label;
		int last;
		int lastTap;
		bool subtractHeld;
		int subtractHeldTime;
		TapBpm *tapBpm;
		TapButton *tapButton;

		void addButton_Lifted(void);
		void addButton_Pressed(void);
		void applyButton_Clicked(void);
		void alterBpm(float bpm);
		void initInterfaceElements(void);
		void resetButton_Clicked(void);
		void subtractButton_Lifted(void);
		void subtractButton_Pressed(void);
		void tapButton_Pressed(void);

	};
}
#endif

