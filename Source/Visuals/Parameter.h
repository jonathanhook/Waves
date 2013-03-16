/**
 * Class:	Parameter
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef Parameter_H
#define Parameter_H

#include <string>

namespace WavesVisuals
{
	class Parameter
	{
	public:
		Parameter(std::string name, float defaultValue = 0.5f, bool disabled = false);
		~Parameter(void);

		float getDefaultValue(void) const;
		const std::string &getName(void) const;
		float getValue(void) const;
		bool isDisabled(void) const;
		void setDefaultValue(float defaultValue);
		void setDisabled(bool disabled);
		void setValue(float value);

	private:
		float defaultValue;
		bool disabled;
		std::string name;
		float value;
	};
}
#endif

