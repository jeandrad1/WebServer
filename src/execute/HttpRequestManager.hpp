#ifndef HTTPREQUESTMANAGER_HPP
# define HTTPREQUESTMANAGER_HPP

#include "HttpRequest.hpp"
#include "../builder/ServerConfig.hpp"

class HttpRequestManager
{
    private:
		std::string method;
		std::string path;
        std::string query_string;
		std::string version;
		std::map<std::string, std::string> headers;
        std::string body;

        void parseRequestLine(const std::string &line);
        void parseHeaders(std::istream &stream);
        void parseBody(std::istream &stream);

    public:
        std::string getHeader(const std::string& key) const;

        void parseHttpRequest(const std::string& raw_request, std::vector<ServerConfig *> servers);
        void parseHttpHeader(const std::string& raw_header);

        bool checkClientMaxBodySize(std::vector<ServerConfig *> servers);
        HttpRequest *buildHttpRequest();

        void requestPrinter();
};

#endif