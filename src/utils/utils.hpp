#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>

bool		isInteger(std::string value);
bool		isURI(const std::string &path);
bool		isURL(const std::string &path);
bool		checkValidErrorCode(int code);
bool		checkCodeIsValid(int code);
std::string	getSubstringBeforeSemicolon(const std::string &value);
bool		checkChars(const std::string &value);
std::string	trim(const std::string &str);
int			countWhitespace(const std::string &str);
bool		checkFilename(const std::string &word);
bool		safe_atoll(const std::string& str, long long& result);

#endif