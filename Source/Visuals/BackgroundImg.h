/**
 * Class:	BackgroundImg
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef BackgroundImg_H
#define BackgroundImg_H

#include "Visuals/Visual.h"

namespace WavesVisuals
{
	class BackgroundImg :
		public Visual
	{
	public:
		BackgroundImg(std::string path, std::string name);
		~BackgroundImg(void);

		void draw(void);
		void update(void);
		virtual Visual *getInstance(void) const = 0;

	private:
		static const std::string	DISPLAY_LIST_KEY;
		static const float			Z_POS;

		unsigned int displayList;
		std::string displayListKey;
		std::string path;
	};
}
#endif

