#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include <string>
#include <map>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include "../utils/colors.hpp"
#include "../utils/utils.hpp"

class HttpRequest {
	private:
		std::string method;
		std::string path;
		std::string version;
		std::map<std::string, std::string> headers;

		//headers value
		long long contentLength;
		std::string host;

	public:
		void parseRequest(const std::string& raw_request);
		std::string getHeader(const std::string& key) const;
		void handleContentLength();
		void handleHost();
		void requestPrinter();
};

#endif