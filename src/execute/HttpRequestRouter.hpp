#ifndef REQUESTROUTER_HPP
#define REQUESTROUTER_HPP

#include "IHttpRequestHandle.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <string>

class HttpRequestRouter : public IHttpRequestHandler
{
	public:
		HttpResponse handleRequest(const HttpRequest& req);

	private:
		HttpResponse handleGet(const HttpRequest& req);
		HttpResponse handlePost(const HttpRequest& req);
		HttpResponse handleDelete(const HttpRequest& req);
		HttpResponse methodNotAllowed();
};

#endif // REQUESTROUTER_HPP
