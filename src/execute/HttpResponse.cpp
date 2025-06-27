#include "HttpResponse.hpp"

void HttpResponse::setStatus(int code, const std::string& message)
{
    statusCode = code;
    statusMessage = message;
}

void HttpResponse::setHeader(const std::string& key, const std::string& value)
{
    headers[key] = value;
}

void HttpResponse::setBody(const std::string& data)
{
    body = data;
}

std::string HttpResponse::buildResponse() const
{
    std::ostringstream response;
    response << "HTTP/1.1 " << statusCode << " " << statusMessage << "\r\n";
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        response << it->first << ": " << it->second << "\r\n";
    }
    response << "\r\n";
    response << body;
    return response.str();
}

int HttpResponse::getStatusCode() const
{
    return statusCode;
}

std::string HttpResponse::getStatusMessage() const
{
    return statusMessage;
}

const std::map<std::string, std::string>& HttpResponse::getHeaders() const
{
    return headers;
}
