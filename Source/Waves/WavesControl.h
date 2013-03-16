/**
 * Class:	WavesControl
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef WavesControl_H
#define WavesControl_H

#include <map>
#include <string>
#include <vector>
#include "FastDelegate.h"
#include "InterfaceElement.h"
#include "JDHUtility/Point2f.h"
#include "JDHUtility/Vector2f.h"

namespace WavesVisuals { class Parameter; }

using namespace fastdelegate;
using namespace WavesVisuals;

namespace Waves
{
	class AddToggleButton;
	class DragButton;
	class RadarSegment;
	class TextScrollMenu;
	class Track;

	class WavesControl : 
		public InterfaceElement
	{
	public:
		typedef FastDelegate1<WavesControl *> FingerLiftedCallback;
		typedef FastDelegate0<> HeightChangedCallback;
		typedef FastDelegate1<WavesControl *> MovedCallback;

		static bool compare(WavesControl *lhs, WavesControl *rhs);

		WavesControl(const Point2f &position, float width, std::string title, vector<Track *> tracks);
		~WavesControl(void);

		void addTrack(Parameter *parameter);
		bool contains(const FingerEventArgs &e) const;
		void fingerAdded(const FingerEventArgs &e);
		void fingerUpdated(const FingerEventArgs &e);
		void fingerRemoved(const FingerEventArgs &e);
		RadarSegment &getSegment(void);
		bool isOnRadar(void) const;
		void render(void);
		unsigned int renderRadarSegmentTexture(void);
		virtual void save(void) = 0;
		void setFingerLiftedCallback(FingerLiftedCallback fingerLifted);
		void setHeightChangedCallback(HeightChangedCallback heightChanged);
		void setMovedCallback(MovedCallback moved);
		void setOnRadar(bool onRadar);
		virtual void update(void);

	protected:
		static const float SCROLL_MENU_HEIGHT;
		static const float SCROLL_MENU_WIDTH;

		AddToggleButton *addButton;
		TextScrollMenu *menu;
		int menuLastInteraction;
		std::string title;
		std::vector<Track *> tracks;

		virtual void addToggleButton_ToggleChanged(bool toggled) = 0;
		void hideMenu(void);
		virtual void menu_ItemSelected(std::string item) = 0;
		void showMenu(vector<std::string> items);

	protected:
		bool onRadar;
		RadarSegment *segment;
		
		virtual void initInterfaceElements(void);

	private:
		static const unsigned int MENU_HIDE_DELAY = 5000;
		static const unsigned int TEXTURE_HEIGHT = 2048;
		static const unsigned int TEXTURE_WIDTH = 2048;
		
		static unsigned int depthBuffer;
		static unsigned int frameBufferObject;
		static unsigned int texture;

		static void setupFbo(void);
		
		FingerLiftedCallback fingerLifted;
		HeightChangedCallback heightChanged;
		MovedCallback moved;
		DragButton *titleBar;

		float getOffsetPlayheadPosition(void);
		void menu_Touched(void);
		void removeTrack(Track *t);
		void titleBar_dragged(const Vector2f &v);
		void titleBar_fingerLifted(void);
		void track_Exited(Track *track);
		void updateHeight(void);
	};
}
#endif

