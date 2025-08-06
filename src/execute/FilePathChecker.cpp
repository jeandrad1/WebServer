#include "FilePathChecker.hpp"
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib> 
#include <iostream>

bool FilePathChecker::fileExists(const std::string& fullPath)
{
    struct stat s;
    return stat(fullPath.c_str(), &s) == 0 && S_ISREG(s.st_mode);
}

bool FilePathChecker::isSafePath(const std::string& base, const std::string& requested)
{
    char realBase[4096];
    char realRequested[4096];

    if (!realpath(base.c_str(), realBase) || !realpath(requested.c_str(), realRequested))
        return false;

    std::string realBaseStr(realBase);
    std::string realRequestedStr(realRequested);
	
    return realRequestedStr.find(realBaseStr) == 0;
}
