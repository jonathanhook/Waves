/**
 * Class:	Button
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef Button_H
#define Button_H

#include <string>
#include "FastDelegate.h"
#include "InterfaceElement.h"

using namespace fastdelegate;
using namespace JDHUtility;

namespace JDHUtility
{
	class GLTexture;
}

namespace Waves
{
	class Button :
		public InterfaceElement
	{
	public:	
		typedef enum
		{
			HORIZONTAL,
			VERTICAL
		} TextDirection;

		typedef FastDelegate0<> ClickedCallback;
		typedef FastDelegate0<> LiftedCallback;
		typedef FastDelegate0<> PressedCallback;

		Button(const Point2f &position, float width, float height);
		~Button(void);

		void fingerAdded(const FingerEventArgs &e);
		void fingerUpdated(const FingerEventArgs &e);
		void fingerRemoved(const FingerEventArgs &e);
		void render(void);
		void setClickedCallback(ClickedCallback clicked);
		void setLiftedCallback(LiftedCallback lifted);
		void setPressedCallback(PressedCallback pressed);
		void setText(const std::string &text);
		void setTextDirection(TextDirection textDirection);

	protected:
		static const float BACKGROUND_OPACITY;
		static const std::string BUTTON_TEXTURE_KEY;
		static const float SELECTED_OPACITY;
		static const float TEXT_BORDER;

		ClickedCallback clicked;
		bool hasTextureIcon;
		LiftedCallback lifted;
		PressedCallback pressed;
		bool selected;
		std::string text;
		TextDirection textDirection;
	};
}
#endif
