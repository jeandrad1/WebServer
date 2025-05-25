#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP
#include <string>
#include <map>
#include <sstream>
#include <iostream>


class HttpResponse
{
    private:
        int statusCode;
        std::string statusMessage;
        std::map<std::string, std::string> headers;
        std::string body;

    public:
        void setStatus(int code, const std::string& message);
        void setHeader(const std::string& key, const std::string& value);
        void setBody(const std::string& data);
        std::string buildResponse() const;
        std::string getBody() const { return body; }

};

#endif