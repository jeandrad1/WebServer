#include "AutoindexStrategy.hpp"

bool AutoindexStrategy::validate(const std::string &value) const
{
	if(value.empty())
		return false;
	if(value == "on;" || value == "off;")
		return true;
	return false;
}
