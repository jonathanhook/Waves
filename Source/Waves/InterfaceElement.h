/**
 * Class:	InterfaceElement
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef InterfaceElement_H
#define InterfaceElement_H

#include <map>
#include <assert.h>
#include "JDHUtility/Point2f.h"
#include "JDHUtility/GLContext.h"
#include "JDHUtility/GLMatrixf.h"
#include "MultiTouchEvents/MultiTouchEventHandler.h"

namespace MultiTouchEvents { class FingerEventArgs; };

using namespace JDHUtility;
using namespace MultiTouchEvents;

namespace Waves
{
	class InterfaceElement :
		public MultiTouchEventHandler
	{
	public:
		InterfaceElement(const Point2f &position, float width, float height);
		~InterfaceElement(void);

		bool contains(const FingerEventArgs &e) const;
		void deleteInterfaceElement(InterfaceElement *e);
		std::map<int, InterfaceElement *> getInterfaceElements(void) const;
		float getHeight(void) const;
		unsigned int getId(void) const;
		float getWidth(void) const;
		Point2f &getPosition(void);
		bool isMarkedForDelete(void) const;
		bool isTransformSaved(void) const;
		void registerInterfaceElement(InterfaceElement *e);
		virtual void render(void) = 0;
		void setDirty(bool dirty);
		void setHeight(float height);
		void setId(unsigned int id);
		void setMarkedForDelete(bool markedForDelete);
		virtual void setPosition(const Point2f &position);
		void setWidth(float width);
		virtual void update(void);
	protected:
		static const float BAR_DIM;
		static const float BORDER_DIM;
		static const float ICON_LINE_WIDTH;
		static const float LINE_WIDTH;
		static const float LINE_OPACITY;
		static const float MASK_OPACITY;
		static const float SPLINE_LINE_WIDTH;
		static const float TEXT_LINE_WIDTH;

		bool dirty;
		unsigned int displayList;
		float height;
		unsigned int id;
		std::map<int, InterfaceElement *> interfaceElements;
		bool markedForDelete;
		GLMatrixf *modelview;
		Point2f position;
		bool transformSaved;
		float width;

		float getLineWidth(float width = LINE_WIDTH) const;
		virtual void initInterfaceElements(void);
		virtual void renderInterfaceElements(void);
		virtual void renderBackgroundMask(void);
		void saveTransform(void);
		virtual void updateInterfaceElements(void);
	};
}

#endif
