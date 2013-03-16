/**
 * Class:	RadButtonGroup
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "RadButton.h"
#include "RadButtonGroup.h"

namespace Waves
{
	/* CONSTRUCTORS */
	RadButtonGroup::RadButtonGroup(void)
	{
	}

	RadButtonGroup::~RadButtonGroup(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void RadButtonGroup::join(RadButton *r)
	{
		members.push_back(r);
	}

	void RadButtonGroup::unToggleOthers(RadButton *r)
	{
		for(std::vector<RadButton *>::iterator it = members.begin(); it != members.end(); it++)
		{
			RadButton *rButton = *it;
			assert(rButton != NULL);

			if(r != rButton)
			{
				rButton->unToggle();
			}
		}
	}
}
