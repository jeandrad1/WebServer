#include "MimeTypeDetector.hpp"

MimeTypeDetector::MimeTypeDetector()
{
    mimeTypes["html"] = "text/html";
    mimeTypes["htm"] = "text/html";
    mimeTypes["css"] = "text/css";
    mimeTypes["js"] = "application/javascript";
    mimeTypes["png"] = "image/png";
    mimeTypes["jpg"] = "image/jpeg";
    mimeTypes["jpeg"] = "image/jpeg";
    mimeTypes["gif"] = "image/gif";
    mimeTypes["txt"] = "text/plain";
    mimeTypes["json"] = "application/json";
    mimeTypes["pdf"] = "application/pdf";
}

std::string MimeTypeDetector::getMimeType(const std::string& path) const
{
    std::string ext = getFileExtension(path);
    std::map<std::string, std::string>::const_iterator it = mimeTypes.find(ext);
    if (it != mimeTypes.end())
        return it->second;
    return "application/octet-stream";
}

std::string MimeTypeDetector::getFileExtension(const std::string& filename) const
{
    std::size_t dot = filename.find_last_of('.');
    if (dot != std::string::npos)
        return filename.substr(dot + 1);
    return "";
}
