#ifndef REQUESTROUTER_HPP
#define REQUESTROUTER_HPP

#include "IHttpRequestHandle.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <string>
# include "EpollManager.hpp"
# include "SocketsManager.hpp"
#include "ResponseFactory.hpp"


class HttpRequestRouter : public IHttpRequestHandler
{
	public:
		HttpRequestRouter() {}
		~HttpRequestRouter() {}
		HttpResponse handleRequest(const HttpRequest& req, const ServerConfig& server);
		static HttpResponse serveFile(const std::string& path, const std::string& virtualPath);

	private:
		HttpResponse generateAutoIndexResponse(const std::string& dirPath, const std::string& requestPath);
		HttpResponse handleGet(const HttpRequest& req, const ServerConfig& server);
		HttpResponse handlePost(const HttpRequest& req, const ServerConfig& server);
		HttpResponse handleDelete(const HttpRequest& req, const ServerConfig& server);
		HttpResponse methodNotAllowed();
		const LocationConfig* findMatchingLocation(const ServerConfig& server, const std::string& path);
		HttpResponse generateErrorResponse(int code, const ServerConfig& server, const LocationConfig* location);

};

#endif
