/**
 * Class:	DragHandle
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef DragHandle_H
#define DragHandle_H

#include "DragButton.h"

namespace Waves
{
	class DragHandle :
		public DragButton
	{
	public:
		typedef enum
		{
			EXTEND,
			REPLICATE
		} HandleType;

		DragHandle(const Point2f &position, float width, float height, HandleType handleType);
		~DragHandle(void);

		void render(void);

	private: 
		static const float ICON_POSITION;

		bool dragHandleDirty;
		unsigned int dragHandleDisplayList;
		HandleType handleType;
	};
}
#endif
