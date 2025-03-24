
#include "../include\strategy\strategies\RootValidationStragegy.hpp"
#include <string>
#include <set>
#include <algorithm>


bool RootValidationStrategy::validate(const std::string &value) const
{
    if (value.empty() || value.length() > 260)
        return false;
    if (hasForbiddenCharacters(value) || isReservedName(value))
        return false;
    return true;
}

bool RootValidationStrategy::hasForbiddenCharacters(const std::string &value) const
{
    #ifdef _WIN32
        static const std::string forbiddenChars = "<>:\"/\\|?*";
        return value.find_first_of(forbiddenChars) != std::string::npos;
    #else
        static const std::string forbiddenChars = "*?[]!&$(){};:'\"\\|<> ";
        return value.find_first_of(forbiddenChars) != std::string::npos || value.find('/') != std::string::npos || value.find('\0') != std::string::npos;
    #endif
}

bool RootValidationStrategy::isReservedName(const std::string &value) const
{
    #ifdef _WIN32
        static const std::set<std::string> reservedNames = {
            "CON", "PRN", "AUX", "NUL",
            "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9",
            "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"
        };
        return reservedNames.find(value) != reservedNames.end();
    #else
        return false;
#endif
}
