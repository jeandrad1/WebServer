#include "ClientMaxBodySizeStrategy.hpp"

bool	ClientMaxBodySizeStrategy::validate(const std::string &value) const
{
	if (value.empty())
	{
		std::cout << "Client_Max_Body_Size Strategy. Value: " << value << "\n";
		return (false);
	}

	if (value == "0")
		return (true);

	std::size_t FirstLetterPos = value.find_first_not_of("1234567890");

	if (FirstLetterPos == 0)
		return (false);
	if (FirstLetterPos == std::string::npos)
		return (false);

	std::string FirstLetter = value.substr(FirstLetterPos);

	if (FirstLetter == ";" || FirstLetter == "k;" || FirstLetter == "K;" \
		|| FirstLetter == "m;" || FirstLetter == "M;" \
		|| FirstLetter == "g;" || FirstLetter == "G;")
		return (true);

	return (false);
}
