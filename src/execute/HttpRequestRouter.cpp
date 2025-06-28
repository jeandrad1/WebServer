#include "HttpRequestRouter.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "HttpRequestRouter.hpp"
#include "MimeTypeDetector.hpp"
#include "FilePathChecker.hpp"
#include <fstream>
#include <sys/stat.h>
#include <dirent.h>

HttpResponse HttpRequestRouter::handleRequest(const HttpRequest& req, const ServerConfig& server) {
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

HttpResponse HttpRequestRouter::serveFile(const std::string& path, const std::string& virtualPath) 
{
    MimeTypeDetector mime;

	int fd = open(path.c_str(), O_RDONLY);
	if (fd < 0)
		return ResponseFactory::createResponse(403);

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
    res.setHeader("Content-Type", mime.getMimeType(virtualPath));
	return res;
}

HttpResponse HttpRequestRouter::handleGet(const HttpRequest& req, const ServerConfig& server) {
	std::string path = req.getPath();
	std::string root = server.root;
	std::string fullPath = root + path;

	if (!FilePathChecker::isSafePath(root, fullPath))
		return ResponseFactory::createResponse(403);

	struct stat s;
	if (stat(fullPath.c_str(), &s) != 0)
		return ResponseFactory::createResponse(404);

	if (S_ISDIR(s.st_mode))
    {
		std::string indexPath = fullPath + "/index.html";
		struct stat indexStat;
        if (stat(indexPath.c_str(), &indexStat) == 0 && S_ISREG(indexStat.st_mode)) 
		{
            HttpResponse response = serveFile(indexPath, "index.html");
            response.setHeader("Connection", "keep-alive");
            response.setHeader("Keep-Alive", "timeout=5, max=100");
            return response;
        }
        else if (server.autoindex) 
		{
            HttpResponse response = generateAutoIndexResponse(fullPath, path);
			response.setHeader("Connection", "keep-alive");
            response.setHeader("Keep-Alive", "timeout=5, max=100");
            return response;
        }
		else
			return ResponseFactory::createResponse(403);
	}
	return serveFile(fullPath, path);
}

HttpResponse HttpRequestRouter::handlePost(const HttpRequest& req, const ServerConfig& server)
{
    // Basic POST behavior: echo back the body.
    // Need to do a lot more
    // Conversion because of the vector of unsigned char
    std::vector<unsigned char> bodyVec = req.getBody();
    std::string body
    (
        static_cast<const char*>(static_cast<const void*>(bodyVec.data())),
        bodyVec.size()
    );
    HttpResponse res = ResponseFactory::createResponse(200, "POST received:\n" + body);
    res.setHeader("Content-Type", "text/plain");
    return res;
}
HttpResponse HttpRequestRouter::handleDelete(const HttpRequest& req, const ServerConfig& server) {
	std::string path = req.getPath();
	std::string root = server.root;
	std::string fullPath = root + path;

	struct stat s;
	if (stat(fullPath.c_str(), &s) != 0)
		return ResponseFactory::createResponse(404);

	if (remove(fullPath.c_str()) != 0)
		return ResponseFactory::createResponse(500);

	return ResponseFactory::createResponse(200, "File deleted");
}

HttpResponse HttpRequestRouter::methodNotAllowed() {
	return ResponseFactory::createResponse(405);
}
