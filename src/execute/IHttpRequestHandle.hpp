#ifndef IHTTPREQUESTHANDLE_HPP
#define IHTTPREQUESTHANDLE_HPP
#include <string>
#include <map>
#include <sstream>
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class IHttpRequestHandler
{
	public:
		virtual HttpResponse handleRequest(const HttpRequest& req) = 0;
		virtual ~IHttpRequestHandler() {}
};

#endif