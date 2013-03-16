/**
 * Class:	Playhead
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef Playhead_H
#define Playhead_H

namespace Waves
{
	class Playhead
	{
	public:
		Playhead(void);
		~Playhead(void);

		float getPosition(void) const;
		void drag(float d);
		void hold(void);
		bool isHeld(void) const;
		void release(void);
		void setPlayheadPosition(float playheadPosition);

	private:
		bool held;
		float playheadPosition;
	};
}
#endif

