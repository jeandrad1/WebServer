#ifndef RESPONSE_FACTORY_HPP
#define RESPONSE_FACTORY_HPP

#include "HttpResponse.hpp"
#include <map>

class ResponseFactory
{
public:
    static HttpResponse createResponse(int statusCode, const std::string& customBody = "");

private:
    static std::string getDefaultMessage(int code);
    static std::string getDefaultBody(int code);
};

#endif
