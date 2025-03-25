#include "../../../include/strategy/strategies/AutoindexStrategy.hpp"

bool AutoindexStrategy::validate(const std::string &value) const
{
	if(value.empty())
		return false;
	if(value == "on" || value == "off")
		return true;
	return false;
}

/* int main()
{
	AutoindexStrategy validator;

	std::string testCases[] = {
		"",       // Vacío (debe fallar)
		"on",      // Válido
		"off",    // Válido
		"offo",    // Válido
	};

	for(int i = 0; i < 4; ++i) 
	{
		std::cout << "Test case: \"" << testCases[i] << "\" --> " << (validator.validate(testCases[i]) ? GREEN"Valid" : RED"Invalid") << WHITE"\n";
	}

	return 0;
} */

