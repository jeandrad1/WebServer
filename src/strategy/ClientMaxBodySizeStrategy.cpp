#include "../../../include/strategy/strategies/ClientMaxBodySizeStrategy.hpp"

bool ClientMaxBodySizeStrategy::validate(const std::string &value) const
{
	if(value.empty())
	{
		std::cout << "Client_Max_Body_Size Strategy. Value: " << value << "\n";
		return false;
	}
	if (value == "0")
		return true;
	std::size_t FirstLetterPos = value.find_first_not_of("1234567890");
	if (FirstLetterPos == 0)
		return false;
	if (FirstLetterPos == std::string::npos)
		return false;
	std::string FirstLetter = value.substr(FirstLetterPos);
	if (FirstLetter == ";" || FirstLetter == "k;" || FirstLetter == "K;" \
		|| FirstLetter == "m;" || FirstLetter == "M;" \
		|| FirstLetter == "g;" || FirstLetter == "G;")
		return true;
	return false;
}

/*int main()
{
	ClientMaxBodySizeStrategy validator;

	std::string testCases[] = {
		"",       // Vacío (debe fallar)
		"0",      // Válido
		"123",    // Válido
		"50k",    // Válido
		"100M",   // Válido
		"2G",     // Válido
		"10kb",   // Inválido (extra 'b')
		"XYZ",    // Inválido (no empieza con número)
		"20a",    // Inválido (unidad no válida)
		"512K",   // Válido
		"300m",   // Válido
		"500g",   // Válido
		"10000000000" // Válido (solo números)
	};

	for(int i = 0; i < 13; ++i) 
	{
		std::cout << "Test case: \"" << testCases[i] << "\" --> " << (validator.validate(testCases[i]) ? RED"Valid" : GREEN"Invalid") << WHITE << std::endl;
	}

	return 0;
}*/