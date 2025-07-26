#include "CGIStrategy.hpp"

#include <sstream>

bool	isURI(const std::string &path);

CGIStrategy::CGIStrategy(void)
{
	this->validExtensions.push_back(".py");
	this->validExtensions.push_back(".cgi");
	this->validExtensions.push_back(".php");
}

bool	isValidExtension(std::vector<std::string> validExtensions, const std::string &suffix)
{
	std::vector<std::string>::iterator ite = validExtensions.end();
	for (std::vector<std::string>::iterator it = validExtensions.begin(); it != ite; it++)
	{
		if (suffix == (*it))
			return (true);
	}
	return (false);
}

bool	CGIStrategy::validate(const std::string &value) const
{
	std::istringstream	iss(value);

	std::string suffix;
	std::string path;

	iss >> suffix;

	if (!isValidExtension(this->validExtensions, suffix))
		return (false);
	
	iss >> path;

	if (!isURI(path))
		return (false);

	return (true);
}