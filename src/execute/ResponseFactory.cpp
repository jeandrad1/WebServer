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

static std::string stripLeadingSlash(const std::string& path)
{
    if (!path.empty() && path[0] == '/')
        return path.substr(1);
    return path;
}


HttpResponse ResponseFactory::generateErrorResponse(int code, const ServerConfig& server, const LocationConfig* location)
{
    std::string errorPagePath;
    
	// Refactor the entire function !!
    
    if (location && location->getErrorPageDirective())
    {
        const std::map<int, t_errorPage*>& errorPages = location->getErrorPages();
                
        std::map<int, t_errorPage*>::const_iterator it = errorPages.find(code);
        
        if (it != errorPages.end() && it->second)
            errorPagePath = it->second->targetPage;
    }
    else if (location)
    {
        const std::map<int, t_errorPage*> serverErrorPages = server.getErrorPages();
        std::map<int, t_errorPage*>::const_iterator it = serverErrorPages.find(code);
        if (it != serverErrorPages.end() && it->second)
            errorPagePath = it->second->targetPage;
    }
    // with inheritance this else should not be necessary
    else
    {
        const std::map<int, t_errorPage*> serverErrorPages = server.getErrorPages();
        std::map<int, t_errorPage*>::const_iterator it = serverErrorPages.find(code);
        if (it != serverErrorPages.end() && it->second)
            errorPagePath = it->second->targetPage;
    }
    if (!errorPagePath.empty())
    {
        std::string fullPath;
		fullPath = stripLeadingSlash(server.getRoot()) + "/" + stripLeadingSlash(errorPagePath);
        
        struct stat s;
        if (stat(fullPath.c_str(), &s) == 0 && S_ISREG(s.st_mode))
        {
            // Refactor this block entirely. it is to put the values 
            std::ifstream file(fullPath.c_str());
            if (file.is_open())
            {
                std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                file.close();
                
                std::string codeStr = to_string(code);
                std::string statusText = getStatusText(code);
                
                size_t scriptPos = content.find("<script");
                if (scriptPos != std::string::npos)
                {
                    std::string injection = "<script>\n";
                    injection += "window.errorCode = " + codeStr + ";\n";
                    injection += "window.errorMessage = '" + statusText + "';\n";
                    injection += "</script>\n";
                    content.insert(scriptPos, injection);
                }
                
                HttpResponse response;
                response.setStatus(code, getStatusText(code));
                response.setHeader("Content-Type", "text/html");
                response.setHeader("Content-Length", to_string(content.length()));
                response.setTextBody(content);
                return response;
			}
        }
        else
    		std::cout << RED << "Error page file not found or not accessible: " << fullPath << RESET << std::endl;
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
    response.setTextBody(body);
    
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
    res.setTextBody(body);

    return res;
}

// Put better error codes and messages in macros or map
// to avoid hardcoding them in the codebase.
std::string ResponseFactory::getDefaultMessage(int code)
{
    switch (code)
	{
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
