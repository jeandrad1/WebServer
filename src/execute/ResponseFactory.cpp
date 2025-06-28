#include "ResponseFactory.hpp"
#include <sstream>

HttpResponse ResponseFactory::createResponse(int code, const std::string& customBody)
{
    HttpResponse res;
    std::string message = getDefaultMessage(code);
    std::string body = customBody.empty() ? getDefaultBody(code) : customBody;
    std::ostringstream oss;
    oss << body.size();

    res.setStatus(code, message);
    res.setHeader("Content-Type", "text/plain");
    res.setHeader("Content-Length", oss.str());
    res.setBody(body);

    return res;
}

// Put better error codes and messages in macros or map
// to avoid hardcoding them in the codebase.
std::string ResponseFactory::getDefaultMessage(int code)
{
    switch (code) {
        case 200: return "OK";
        case 400: return "Bad Request";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 414: return "URI Too Long";
        case 500: return "Internal Server Error";
        case 501: return "Not Implemented";
        default:  return "Unknown";
    }
}

std::string ResponseFactory::getDefaultBody(int code)
{
    std::ostringstream oss;
    oss << code << " " << getDefaultMessage(code);
    return oss.str();
}
