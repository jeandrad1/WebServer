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

HttpResponse HttpRequestRouter::handleRequest(const HttpRequest& req, const ServerConfig& server)
{
    std::string method = req.getMethod();

    if (method == "GET")       return handleGet(req, server);
    else if (method == "POST") return handlePost(req, server);
    else if (method == "DELETE") return handleDelete(req, server);
    else                       return methodNotAllowed();
}

HttpResponse HttpRequestRouter::generateAutoIndexResponse(const std::string& dirPath, const std::string& requestPath)
{
    DIR* dir = opendir(dirPath.c_str());
    if (!dir)
        return ResponseFactory::createResponse(500);

    std::string body = "<html><body><h1>Index of " + requestPath + "</h1><ul>";

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        std::string name = entry->d_name;
        if (name == ".") continue;
        body += "<li><a href=\"" + requestPath;
        if (requestPath[requestPath.length() - 1] != '/')
            body += "/";
        body += name + "\">" + name + "</a></li>";
    }
    body += "</ul></body></html>";
    closedir(dir);

    HttpResponse res = ResponseFactory::createResponse(200, body);
    res.setHeader("Content-Type", "text/html");
    return res;
}

HttpResponse HttpRequestRouter::serveFile(const std::string& filePath, const std::string& urlPath, const ServerConfig &server) 
{
    MimeTypeDetector mime;

    int fd = open(filePath.c_str(), O_RDONLY);
    if (fd < 0)
    {
        const LocationConfig* location = HttpRequestRouter::findMatchingLocation(server,urlPath);
        ResponseFactory factory;
        return factory.generateErrorResponse(403, server, location, urlPath);
    }
    std::string body;
    char buffer[4096];
    ssize_t bytes;
    while ((bytes = read(fd, buffer, sizeof(buffer))) > 0)
    {
        body.append(buffer, bytes);
    }
    close(fd);

    if (bytes < 0)
        return ResponseFactory::createResponse(500);

    HttpResponse res = ResponseFactory::createResponse(200, body);
    res.setHeader("Content-Type", mime.getMimeType(filePath));
    return res;
}

HttpResponse HttpRequestRouter::handleGet(const HttpRequest& req, const ServerConfig& server)
{
    std::string path = req.getPath();
    
    const LocationConfig* location = findMatchingLocation(server, path);
    
    std::string root;
    std::string indexFile;
	std::vector<std::string> server_indexes = server.getIndex(); // default, change in the future to take all the strings

	std::string cleanPath = path;
    
    if (location)
    {
        root = location->getRoot().empty() ? server.getRoot() : location->getRoot();

        if (path.find(location->getLocationPath()) == 0)
		{
            cleanPath = path.substr(location->getLocationPath().length());
            if (cleanPath.empty())
                cleanPath = "/";
            else if (cleanPath[0] != '/')
                cleanPath = "/" + cleanPath;
        }

        if (!location->getIndex().empty())
            indexFile = location->getIndex()[0]; // maybe future change to handle multiple index files
		else if (!server_indexes.empty())
            indexFile = server_indexes[0]; // Use server's index as fallback

    }
    else
	{
        root = server.getRoot();
		cleanPath = path;
		if (!server_indexes.empty())
            indexFile = server_indexes[0];

	}
    std::string fullPath = root + cleanPath;
    
    if (!FilePathChecker::isSafePath(root, fullPath))
	{
		ResponseFactory factory;
        return factory.generateErrorResponse(403, server, location, path);
	}
    struct stat s;
    if (stat(fullPath.c_str(), &s) != 0)
	{
		ResponseFactory factory;
        return factory.generateErrorResponse(404, server, location, path);
	}
    if (S_ISDIR(s.st_mode))
    {
        // Try to find any available index file from the list
        std::string indexPath;
        bool indexFound = false;
        
        // If we have an indexFile, try it first
        if (!indexFile.empty())
        {
            indexPath = fullPath + "/" + indexFile;
            struct stat indexStat;
            if (stat(indexPath.c_str(), &indexStat) == 0 && S_ISREG(indexStat.st_mode))
                indexFound = true;
        }
        
        // If first index file not found, try all server indexes
        if (!indexFound && !server_indexes.empty())
        {
            for (std::vector<std::string>::const_iterator it = server_indexes.begin(); 
                 it != server_indexes.end(); ++it)
            {
                indexPath = fullPath + "/" + *it;
                struct stat indexStat;
                if (stat(indexPath.c_str(), &indexStat) == 0 && S_ISREG(indexStat.st_mode))
                {
                    indexFound = true;
                    break;
                }
            }
        }
        
        if (indexFound)
        {
            HttpResponse response = serveFile(indexPath, path, server);
            response.setHeader("Connection", "keep-alive");
            response.setHeader("Keep-Alive", "timeout=5, max=100");
            return response;
        }
        else if (server.getAutoIndex()) {
            HttpResponse response = generateAutoIndexResponse(fullPath, path);
            response.setHeader("Connection", "keep-alive");
            response.setHeader("Keep-Alive", "timeout=5, max=100");
            return response;
        }
        else
        {
            ResponseFactory factory;
            return factory.generateErrorResponse(403, server, location, path);
        }
    }

    return serveFile(fullPath, path, server);
}

const LocationConfig* HttpRequestRouter::findMatchingLocation(const ServerConfig& server, const std::string& path)
{
    const LocationConfig* bestMatch = NULL;
    size_t longestMatch = 0;
    
    const std::vector<LocationConfig*> &location = server.getLocations();
    for (std::vector<LocationConfig*>::const_iterator it = location.begin(); it != server.getLocations().end(); ++it)
    {
        LocationConfig* loc = *it; 
        if (path.find(loc->getLocationPath()) == 0 && loc->getLocationPath().length() > longestMatch)
        {
            bestMatch = loc;
            longestMatch = loc->getLocationPath().length();
        }
    }
    
    return bestMatch;
}

HttpResponse HttpRequestRouter::handlePost(const HttpRequest& req, const ServerConfig& server)
{
    std::string path = req.getPath();
    const LocationConfig* location = findMatchingLocation(server, path);

    std::string root;
    std::string cleanPath = path;

    if (location)
    {
        root = location->getRoot().empty() ? server.getRoot() : location->getRoot();

        if (path.find(location->getLocationPath()) == 0)
        {
            cleanPath = path.substr(location->getLocationPath().length());
            if (cleanPath.empty())
                cleanPath = "/";
            else if (cleanPath[0] != '/')
                cleanPath = "/" + cleanPath;
        }
    }
    else
        root = server.getRoot();

    std::string fullPath = root + cleanPath;

    if (!FilePathChecker::isSafePath(root, fullPath))
    {
		ResponseFactory factory;
        return factory.generateErrorResponse(403, server, location, path);
    }
    struct stat s;
    if (stat(fullPath.c_str(), &s) == 0 && S_ISDIR(s.st_mode))
    {
		ResponseFactory factory;
        return factory.generateErrorResponse(403, server, location, path);
    }
    if (access(fullPath.c_str(), F_OK) == 0)
	{
    	ResponseFactory factory;
        return factory.generateErrorResponse(409, server, location, path);
    }
    std::ofstream out(fullPath.c_str(), std::ios::binary);
	
	if (!out)
    {
		ResponseFactory factory;
        return factory.generateErrorResponse(409, server, location, path);
    }
	std::string body = to_string(req.getBody());
	if (body.empty())
    {
		ResponseFactory factory;
        return factory.generateErrorResponse(400, server, location, path);
    }
	out.write(body.c_str(), body.size());
    out.close();

    // Succces: 201 Created
    HttpResponse res;
    res.setStatus(201, "Created");
    res.setHeader("Content-Length", "0");
    res.setHeader("Connection", "keep-alive");
    res.setHeader("Keep-Alive", "timeout=5, max=100");

    return res;
}

HttpResponse HttpRequestRouter::handleDelete(const HttpRequest& req, const ServerConfig& server)
{
	std::string path = req.getPath();
	std::string root = server.getRoot();
	std::string fullPath = root + path;

	struct stat s;
	if (stat(fullPath.c_str(), &s) != 0)
		return ResponseFactory::createResponse(404);

	if (remove(fullPath.c_str()) != 0)
		return ResponseFactory::createResponse(500);

	return ResponseFactory::createResponse(200, "File deleted");
}

HttpResponse HttpRequestRouter::methodNotAllowed()
{
	return ResponseFactory::createResponse(405);
}
