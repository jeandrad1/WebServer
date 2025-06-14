#include "HttpRequestManager.hpp"
#include <cstdio>

std::string HttpRequestManager::getHeader(const std::string& key) const 
{
	std::map<std::string, std::string>::const_iterator it = headers.find(key);
	if (it != headers.end())
	{
		std::cout << "Existe " << it->first << "\n";
		return it->second;
	}
	std::cout << "No existe " << key << "\n";
	return "";
}

void HttpRequestManager::parseRequest(const std::string& raw_request)
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
			headers[to_lowercase(key)] = value;
		}
		else
			throw std::runtime_error(RED "Header error: " YELLOW + line + "\n\t->" + GREEN" NO SEMICOLON" + WHITE);
	}
	if (stream.peek() != EOF)
	{
		std::ostringstream body_stream;
		body_stream << stream.rdbuf();
		body = body_stream.str();
	}
}

void	HttpRequestManager::requestPrinter()
{
	std::cout << CYAN"METHOD:\t " << WHITE << method << "\n";
	std::cout << CYAN"PATH:\t " << WHITE << path << "\n";
	std::cout << CYAN"VERSION: " << WHITE << version << "\n";

	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
	{
		std::cout << (*it).first << YELLOW" -> " << WHITE << (*it).second << "\n";
	}
	if (!body.empty())
	{
		std::cout << CYAN"BODY:\n" WHITE;
		std::cout << body << "\n";
	}
	else
		std::cout << CYAN"NO BODY\n" WHITE;
}

HttpRequest *HttpRequestManager::builderHeadersRequest()
{
	HttpRequest *request = new HttpRequest(method, path, version);
	request->handleContentLength(getHeader("content-length"));
	request->handleContentType(getHeader("content-type"));
	request->handleHost(getHeader("host"));
	request->handleConnection(getHeader("connection"));

	return(request);
}