#include "IndexStrategy.hpp"

int			countWhitespace(const std::string &str);
bool		checkChars(const std::string &value);
bool		checkFilename(const std::string &word);
std::string	getSubstringBeforeSemicolon(const std::string &value);
std::string	trim(const std::string &str);

bool	split(const std::string &value)
{
	std::istringstream iss(value);
	std::string word;
	while (iss >> word)
	{
		if (!checkFilename(word))
			return (false);
	}
	return (true);
}

bool	IndexStrategy::validate(const std::string &value) const
{
	if (value.empty())
		return (false);

	if (value[value.size() - 1] != ';')
		return (false);

	std::string real_value = getSubstringBeforeSemicolon(value);

	if (!checkChars(real_value))
		return (false);

	if (countWhitespace(real_value) == (int)real_value.length())
		return (false);

	std::string trimmed_value = trim(real_value);
	if (real_value.empty())
		return (false);

	return (split(trimmed_value));
}
