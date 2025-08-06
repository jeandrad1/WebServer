#include "utils.hpp"

long long hexToDecimal(const std::string& hexStr) 
{
	long long result = 0;
	size_t i = 0;

	for (; i < hexStr.length(); ++i) 
	{
		char c = hexStr[i];
		int value = 0;

		if (c >= '0' && c <= '9') 
		{
			value = c - '0';
		} 
		else if (c >= 'A' && c <= 'F')
		{
			value = c - 'A' + 10;
		}
		else if (c >= 'a' && c <= 'f')
		{
			value = c - 'a' + 10;
		} 
		else 
		{
			std::cerr << "Error: carácter no válido en la cadena hexadecimal: " << c << std::endl;
			return -1;
		}

		result = result * 16 + value;
	}

	return result;
}

bool safe_atoll(const std::string& str, long long& result) {
    char* end;
    errno = 0;

    const char* cstr = str.c_str();
    result = std::strtoll(cstr, &end, 10);

    if (errno == ERANGE)
        return false;
    if (end == cstr)
        return false;
    while (*end != '\0') {
        if (!std::isspace(*end))
            return false;
        ++end;
    }
    return true;
}

bool	isInteger(std::string value)
{
	if (value.empty())
		return (false);

	for (size_t i = 0; i < value.size(); ++i)
	{
		if (!isdigit(value[i]))
			return (false);
	}
	return (true);
}

bool	isURI(const std::string &path)
{
	if (path[0] == '/')
    	return (true);
	return (false);
}

bool	isURL(const std::string &path)
{
	if (path.find("http://") == 0 || path.find("https://") == 0)
		return (true);
	return (false);
}

bool	checkValidErrorCode(int code)
{
	if (code >= 400 && code <= 599)
		return (true);
	return (false);
}

bool	checkCodeIsValid(int code)
{
	if (code >= 100 && code <= 599)
		return (true);
	return (false);
}

std::string	getSubstringBeforeSemicolon(const std::string &value)
{
	if (value.empty() || value[value.size() - 1] != ';')
		return "";

	return value.substr(0, value.size() - 1);
}

bool	checkChars(const std::string &value)
{
	for (size_t i = 0; i < value.size(); i++)
	{
		char c = value[i];
		if (c > 31 && c < 127)
			return true;
	}
	return false;
}

std::string	trim(const std::string &str)
{
	if (str.size() == 0)
		return "";

	size_t	first = str.find_first_not_of(" \t\r\n");
	if (first == std::string::npos)
		return "";

	size_t	last = str.find_last_not_of(" \t\r\n");
	return str.substr(first, last - first + 1);
}

std::string	trimSpaces(const std::string &str)
{
	if (str.size() == 0)
		return "";

	size_t	first = str.find_first_not_of(" ");
	if (first == std::string::npos)
		return "";

	size_t	last = str.find_last_not_of(" ");
	return str.substr(first, last - first + 1);
}

int	countWhitespace(const std::string &str)
{
	int	count = 0;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (std::isspace(str[i]))
			count++;
	}
	return count;
}

bool	checkFilename(const std::string &word)
{
	return (word.size() >= 5 && word.substr(word.size() - 5) == ".html")
		|| (word.size() >= 4 && word.substr(word.size() - 4) == ".php");
}

std::vector<std::string> split_str(const std::string &str, const std::string &delimiter)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos)
    {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    tokens.push_back(str.substr(start));
    return tokens;
}

static char to_lower_char(char c) {
    return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
}

std::string to_lowercase(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), to_lower_char);
    return result;
}
