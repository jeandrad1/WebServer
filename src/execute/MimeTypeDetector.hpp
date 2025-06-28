#ifndef MIME_TYPE_DETECTOR_HPP
#define MIME_TYPE_DETECTOR_HPP

#include <string>
#include <map>

class MimeTypeDetector
{
public:
    MimeTypeDetector();
    std::string getMimeType(const std::string& path) const;

private:
    std::map<std::string, std::string> mimeTypes;
    std::string getFileExtension(const std::string& filename) const;
};

#endif
