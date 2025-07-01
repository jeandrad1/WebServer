#include "ResponseFactory.hpp"
#include <sstream>
#include "HttpRequestRouter.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "HttpRequestRouter.hpp"
#include "MimeTypeDetector.hpp"
#include "FilePathChecker.hpp"
#include <fstream>
#include <sys/stat.h>
#include <dirent.h>
#include <sstream>
#include "../utils/to_string.hpp"


HttpResponse ResponseFactory::generateErrorResponse(int code, const ServerConfig& server, const LocationConfig* location)
{
    std::string errorPagePath;
    
    if (location && location->getErrorPageDirective())
    {
        const std::map<int, t_errorPage*>& errorPages = location->getErrorPages();
        std::map<int, t_errorPage*>::const_iterator it = errorPages.find(code);
        
        if (it != errorPages.end() && it->second)
            errorPagePath = it->second->targetPage;
		
		std::cout<<RED<<"The error page path is:"<<errorPagePath<<RESET<<std::endl;
	
	}
    
    if (!errorPagePath.empty())
    {
        std::string fullPath = server.root + "/" + errorPagePath;
        struct stat s;
        if (stat(fullPath.c_str(), &s) == 0 && S_ISREG(s.st_mode))
            return HttpRequestRouter::serveFile(fullPath, errorPagePath);
    }
    return ResponseFactory::createBasicErrorResponse(code);
}

HttpResponse ResponseFactory::createBasicErrorResponse(int code)
{
    std::string statusText = getStatusText(code);
    std::string body = "<html><body><h1>" + to_string(code) + " " + statusText + "</h1></body></html>";
    
    HttpResponse response;
    response.setStatus(code, statusText);
    response.setHeader("Content-Type", "text/html");
    response.setHeader("Content-Length", to_string(body.length()));
    response.setBody(body);
    
    return response;
}

std::string ResponseFactory::getStatusText(int code)
{
    switch (code)
	{
        case 400: return "Bad Request";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 409: return "Conflict";
        case 413: return "Payload Too Large";
        case 499: return "Request Timeout";
        case 500: return "Internal Server Error";
        default: return "Error";
    }
}


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
