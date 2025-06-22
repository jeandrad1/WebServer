#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include <string>
#include <map>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "../utils/colors.hpp"
#include "../utils/utils.hpp"

class HttpRequest 
{
	private:

		std::string method;
		std::string path;
		std::string version;

		long long	contentLength;
		std::string contentType;
		std::string host;
		std::string serverName;
		std::string serverPort;
		bool		connection;

		std::vector<unsigned char> body;

		HttpRequest();
	
	public:

		HttpRequest(std::string src_method, std::string src_path, std::string src_version);
		~HttpRequest();

		void handleContentLength(std::string content_length_str);
		void handleContentType(std::string content_type_str);
		void handleHost(const std::string &host_str);
		void handleConnection(std::string connection_str);

		void handleBody(std::string body);

		//getters
		std::string getMethod() const;
		std::string getPath() const;
		std::string getVersion() const;
		std::string getHost() const;
		std::string getServerName() const;
		std::string getServerPort() const;
		long long	getContentLenght();
		std::vector<unsigned char> getBody() const;

		//printer
		void HttpRequestPrinter();

};

#endif