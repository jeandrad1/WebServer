#ifndef RESPONSE_FACTORY_HPP
#define RESPONSE_FACTORY_HPP

#include "HttpResponse.hpp"
#include "../builder/ServerConfig.hpp"
#include "../builder/LocationConfig.hpp"
#include <map>

class ResponseFactory
{
public:
    static HttpResponse createResponse(int statusCode, const std::string& customBody = "");
	std::string getStatusText(int code);
	HttpResponse createBasicErrorResponse(int code);
	HttpResponse generateErrorResponse(int code, const ServerConfig& server, const LocationConfig* location);


private:
    static std::string getDefaultMessage(int code);
    static std::string getDefaultBody(int code);
};

#endif
