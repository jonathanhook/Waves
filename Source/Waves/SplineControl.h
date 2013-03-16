/**
 * Class:	SplineControl
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef SplineControl_H
#define SplineControl_H

#include <string>
#include <vector>
#include "ControlPoint.h"
#include "FastDelegate.h"
#include "MultiTouchEvents/FingerEventArgs.h"
#include "JDHUtility/GlutInclude.h"

using namespace fastdelegate;

namespace Waves
{
	class FingerHeldCursor;
	class SplinePreset;

	class SplineControl :
		public InterfaceElement
	{
	public:
		static const int CONTROL_POINT_NOT_FOUND = -2;
		static const int ADD_CONTROL_POINT = -1;

		typedef FastDelegate0<> ExitedCallback;

		SplineControl(const Point2f &position, float width, float height, bool presetSaved = false);
		~SplineControl(void);

		bool contains(const FingerEventArgs &e) const;
		bool controlPointContains(const FingerEventArgs &e) const;
		void exitButton_Clicked(void);
		void fingerAdded(const FingerEventArgs &e);
		void fingerUpdated(const FingerEventArgs &e);
		void fingerRemoved(const FingerEventArgs &e);
		std::vector<ControlPoint*> getControlPoints(void);
		unsigned int getPresetId(void) const;
		virtual float getValue(float beat) = 0;
		bool isPresetSaved(void) const;
		virtual void loadPreset(SplinePreset *preset) = 0;
		void registerControlPoint(const Point2f &p);
		void reset(void);
		virtual void render(void);
		virtual void renderSpline(void) = 0;
		virtual void savePreset(void) = 0;
		void setExitedCallback(ExitedCallback exited);
		void setPresetId(unsigned int presetId);
		void setPresetSaved(bool presetSaved);
		void update(void);

	protected:
		static const float INITIAL_VALUE;
		static const float LEGEND_DIM;
		static const unsigned int LEGEND_BEAT_SUBDIVISIONS = 4;
		static const float LEGEND_SUBBEAT_DIM;
		static const float UPSAMPLE_FACTOR;

		std::vector<ControlPoint *> controlPoints;
		bool presetSaved;
		unsigned int presetId;
		bool splineDirty;
		unsigned int splineDisplayList;

		float constrainXVal(float val);
		float constrainYVal(float val);
		unsigned int fullLengthDisplayList;
		bool fullLengthDirty;
		float getValueFromControlPoint(ControlPoint &cp);
		virtual void renderSplineControl(float SplineControlWidth) = 0;
		void savePreset(unsigned int presetType);
		
	private:
		static const unsigned int BEAT_SNAP = 4;
		static const float CONTROL_POINT_SIZE;
		static const float FINGER_RANGE_FACTOR;
		static const float HELD_CURSOR_SIZE;
		static const unsigned int NEW_CURSOR_CREATE_DELAY = 100;
		static const float NEW_CURSOR_MOVEMENT_THRESHOLD;
		static const unsigned int NEW_CURSOR_PERIOD = 500;
		static const float RANGE;
		static const float TEXT_OPACITY;

		typedef struct
		{
			Point2f currentPosition;
			Point2f downPosition;
			int downTime;
			FingerHeldCursor *heldCursor;
		} HeldFinger;

		ExitedCallback exited;
		std::map<unsigned int, HeldFinger> heldFingers;

		void checkForAddGesture(void);
		void controlPoint_FingerDragged(ControlPoint &cp, const FingerEventArgs &e);
		void controlPoint_FingerUp(ControlPoint &cp, const FingerEventArgs &e);
		void heldFinger_FingerHeld(FingerHeldCursor *fingerHeld);
		bool isClose(const Point2f &p);
		void snapControlPoint(ControlPoint &cp);

	};
}
#endif
