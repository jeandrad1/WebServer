#ifndef HTTPREQUESTMANAGER_HPP
# define HTTPREQUESTMANAGER_HPP

#include "HttpRequest.hpp"

class HttpRequestManager
{
    private:
		std::string method;
		std::string path;
		std::string version;
		std::map<std::string, std::string> headers;
        std::string body;
    public:
        std::string getHeader(const std::string& key) const;
        void parseRequest(const std::string& raw_request);

        HttpRequest *builderHeadersRequest();

        void requestPrinter();
};

#endif