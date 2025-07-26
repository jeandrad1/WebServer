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

void HttpResponse::setBinaryBody(const std::vector<unsigned char>& data)
{
    body = data;
	isBinary = true;
}

void HttpResponse::setTextBody(const std::string& data)
{
    body.assign(data.begin(), data.end());
	isBinary = false;
}

const std::vector<unsigned char>& HttpResponse::getBodyBinary() const
{
    return body;
}

std::string HttpResponse::getBody() const 
{
    return std::string(body.begin(), body.end());
}


std::string HttpResponse::buildResponse() const
{
    std::ostringstream response;
    response << "HTTP/1.1 " << statusCode << " " << statusMessage << "\r\n";
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        response << it->first << ": " << it->second << "\r\n";
    }
    response << "\r\n";
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

bool HttpResponse::bodyIsBinary() const
{
    return isBinary;
}