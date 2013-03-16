/**
 * Class:	VisualWavesControl
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef VisualWavesControl_H
#define VisualWavesControl_H

#include "Visuals/Visual.h"
#include "VisualWavesControl.h"
#include "WavesControl.h"

using namespace WavesVisuals;

namespace Waves
{
	class VisualWavesControl :
		public WavesControl
	{
	public:
		typedef FastDelegate1<VisualWavesControl *> ClosedCallback;
		typedef FastDelegate1<VisualWavesControl *> SavedCallback;

		static VisualWavesControl *load(std::string file);
		static std::map<unsigned int, VisualWavesControl *> loadAll(void);

		VisualWavesControl(const Point2f &position, float width, Visual *visual, vector<Track *> tracks, std::string nickname = "", bool forOutput = false);
		~VisualWavesControl(void);

		const std::string &getNickname(void) const;
		Visual *getVisual(void) const;
		bool isForOutput(void) const;
		void renderVisual(void);
		void save(void);
		void setClosedCallback(ClosedCallback closed);
		void setSavedCallback(SavedCallback saved);
		void update(void);
		void updateParameterValues(void);

	private:
		static const float ITEM_HEIGHT;
		static const char *PATH;
		static const char *XML_ID;
		static const char *XML_FFT_PRESET;
		static const char *XML_FOR_OUTPUT;
		static const char *XML_HEIGHT;
		static const char *XML_PARAMETER;
		static const char *XML_POSITION;
		static const char *XML_MODE;
		static const char *XML_NICKNAME;
		static const char *XML_TEMPORAL_PRESET;
		static const char *XML_TRACK;
		static const char *XML_VISUAL;
		static const char *XML_WAVES_CONTROL;
		static const char *XML_WIDTH;
		static const char *XML_X;
		static const char *XML_Y;

		ClosedCallback closed;
		bool forOutput;
		std::string nickname;
		SavedCallback saved;
		Visual *visual;

		void addToggleButton_ToggleChanged(bool toggled);
		void controlPanel_Closed(void);
		void controlPanel_CueChanged(bool selected);
		void controlPanel_Saved(void);
		void initInterfaceElements(void);
		void menu_ItemSelected(std::string item);
	};
}
#endif

