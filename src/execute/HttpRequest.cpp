#include "HttpRequest.hpp"

std::string HttpRequest::getHeader(const std::string& key) const 
{
	std::map<std::string, std::string>::const_iterator it = headers.find(key);
	if (it != headers.end())
		return it->second;
	return "";
}

void HttpRequest::parseRequest(const std::string& raw_request)
{
	std::istringstream stream(raw_request);
	std::string line;

	std::getline(stream, line);
	size_t method_end = line.find(' ');
	size_t path_end = line.find(' ', method_end + 1);
	method = line.substr(0, method_end);
	if (method != "GET" && method != "POST" && method != "DELETE")
		throw std::runtime_error(RED "HTTP method no soported: " YELLOW + method + WHITE);
	path = line.substr(method_end + 1, path_end - method_end - 1);
	version = line.substr(path_end + 1);

	while (std::getline(stream, line) && line != "\r") 
	{
		size_t colon = line.find(':');
		if (colon != std::string::npos) 
		{
			std::string key = line.substr(0, colon);
			if (!key.size())
				throw std::runtime_error(RED "Header error: " YELLOW + line + "\n\t->" + GREEN" KEY EMPTY" + WHITE);
			std::string value = line.substr(colon + 1);
			key.erase(key.find_last_not_of(" \r") + 1);
			value.erase(0, value.find_first_not_of(" "));
			value.erase(value.find_last_not_of("\r") + 1);
			headers[key] = value;
		}
		else
			throw std::runtime_error(RED "Header error: " YELLOW + line + "\n\t->" + GREEN" NO SEMICOLON" + WHITE);
	}
	handleContentLength();
}

void	HttpRequest::handleContentLength()
{
	std::string content_length_str = getHeader("Content-Length");
	if (!content_length_str.empty()) {
		safe_atoll(content_length_str, contentLength);
	}
}

void	HttpRequest::handleHost()
{
	std::string host_str = getHeader("Host");
}

void HttpRequest::handleBody()
{
	if (contentLength > 0 )
	{
		if (body.size() < static_cast<size_t>(contentLength))
			throw std::runtime_error(RED "Body error: BODY SIZE MISMATCH" WHITE);
		if (body.size() > static_cast<size_t>(contentLength))
			body = "";
			std::cout << RED "Body error: BODY SIZE MISMATCH" WHITE << std::endl;
	}
}

HeaderField stringToHeaderField(const std::string& key)
{
	if (key == "method") return HEADER_METHOD;
	else if (key == "path") return HEADER_PATH;
	else if (key == "version") return HEADER_VERSION;
	else if (key == "host") return HEADER_HOST;
	else if (key == "body") return HEADER_BODY;
	else if (key == "content-Length") return HEADER_CONTENT_LENGTH;
	return HEADER_UNKNOWN;
}

std::string HttpRequest::getStringValue(std::string key)
{
	// First check in headers
	std::map<std::string, std::string>::iterator it = headers.find(key);
	if (it != headers.end())
		return it->second;

	// Then check internal fields via enum switch
	switch (stringToHeaderField(key)) {
		case HEADER_METHOD:
			return method;
		case HEADER_PATH:
			return path;
		case HEADER_VERSION:
			return version;
		case HEADER_HOST:
			return host;
		case HEADER_BODY:
			return body;
		case HEADER_CONTENT_LENGTH:
			if (contentLength > 0)
			{
				std::ostringstream oss;
				oss << contentLength;
				return oss.str();
			} 
			else
				return "";
		default:
			return "";
	}
}

void HttpRequest::generateRequest(const std::string& raw_request)
{
		HttpRequest::parseRequest(raw_request);
		HttpRequest::handleContentLength();
		HttpRequest::handleHost();
		HttpRequest::handleBody();
}


void	HttpRequest::requestPrinter()
{
	std::cout << CYAN"METHOD:\t " << WHITE << method << "\n";
	std::cout << CYAN"PATH:\t " << WHITE << path << "\n";
	std::cout << CYAN"VERSION: " << WHITE << version << "\n";

	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
	{
		std::cout << (*it).first << YELLOW" -> " << WHITE << (*it).second << "\n";
	}
}

/*
#include <fstream>

int main(int argc, char **argv)
{
    HttpRequest test;
    std::ifstream archivo(argv[1]);
    if (!archivo) {
        throw std::runtime_error("No se pudo abrir el archivo.");
    }

    std::stringstream buffer;
    buffer << archivo.rdbuf();
	try
	{
		test.parseRequest(buffer.str());
		test.handleBody();
		test.requestPrinter();
		std::cout << "BODY:\n" << test.getStringValue("body") << "\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
*/