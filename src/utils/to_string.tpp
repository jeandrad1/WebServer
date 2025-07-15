#include <sstream>
#include <vector>

template <typename T>
std::string to_string(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

template <>
inline std::string to_string<bool>(const bool& value)
{
    return value ? "true" : "false";
}

template <typename CharType>
std::string to_string(const std::vector<CharType>& value)
{
    std::ostringstream oss;
    for (typename std::vector<CharType>::const_iterator it = value.begin(); 
         it != value.end(); ++it)
    {
        oss << static_cast<char>(*it);
    }
    return oss.str();
}