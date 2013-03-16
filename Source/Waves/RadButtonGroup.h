/**
 * Class:	RadButtonGroup
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef RadButtonGroup_H
#define RadButtonGroup_H

#include <vector>

namespace Waves
{
	class RadButton;

	class RadButtonGroup
	{
	public:
		RadButtonGroup(void);
		~RadButtonGroup(void);

		void join(RadButton *r);
		void unToggleOthers(RadButton *r);

	private:
		std::vector<RadButton *> members;
	};
}
#endif

