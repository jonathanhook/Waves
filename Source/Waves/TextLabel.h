/**
 * Class:	TextLabel
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef TextLabel_H
#define TextLabel_H

#include <string>
#include "InterfaceElement.h"
#include "JDHUtility/Point2f.h"

using namespace JDHUtility;

namespace Waves
{
	class TextLabel :
		public InterfaceElement
	{
	public:
		TextLabel(const Point2f &position, float width, float height, std::string text);
		~TextLabel(void);

		bool contains(const FingerEventArgs &e) const;
		void fingerAdded(const FingerEventArgs &e);
		void fingerUpdated(const FingerEventArgs &e);
		void fingerRemoved(const FingerEventArgs &e);
		void render(void);
		void setCentered(bool centered);
		void setText(std::string text);

	private:
		bool centered;
		std::string text;

	};
}
#endif

