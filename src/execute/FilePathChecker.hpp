#ifndef FILE_PATH_CHECKER_HPP
#define FILE_PATH_CHECKER_HPP

#include <string>

class FilePathChecker
{
public:
    static bool isSafePath(const std::string& base, const std::string& requested);
    static bool fileExists(const std::string& fullPath);
};

#endif
