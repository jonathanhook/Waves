/**
 * Class:	Parameter
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "Visuals/Parameter.h"

namespace WavesVisuals
{
	/* CONSTRUCTORS */
	Parameter::Parameter(std::string name, float defaultValue, bool disabled)
	{
		this->name = name;
		this->defaultValue = defaultValue;
		this->value = defaultValue;
		this->disabled = disabled;
	}

	Parameter::~Parameter(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	float Parameter::getDefaultValue(void) const
	{
		return defaultValue;
	}

	const std::string &Parameter::getName(void) const
	{
		return name;
	}

	float Parameter::getValue(void) const
	{
		return value;
	}

	bool Parameter::isDisabled(void) const
	{
		return disabled;
	}

	void Parameter::setDefaultValue(float defaultValue)
	{
		this->defaultValue = defaultValue;
	}

	void Parameter::setDisabled(bool disabled)
	{
		this->disabled = disabled;
	}

	void Parameter::setValue(float value)
	{
		this->value = value;
	}
}
