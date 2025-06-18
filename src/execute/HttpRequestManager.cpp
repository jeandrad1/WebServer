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

void	HttpRequestManager::parseHttpHeader(const std::string& raw_header)
{
	std::istringstream stream(raw_header);
	std::string line;

	std::getline(stream, line);

	parseHeaders(stream);
}

void HttpRequestManager::parseHttpRequest(const std::string& raw_request)
{
	std::istringstream stream(raw_request);
	std::string line;

	std::getline(stream, line);
	
	parseRequestLine(line);

	parseHeaders(stream);

	parseBody(stream);
}

void HttpRequestManager::parseRequestLine(const std::string &line)
{
	size_t method_end = line.find(' ');
    if (method_end == std::string::npos)
        throw std::runtime_error("Malformed request line: missing method");

	size_t path_end = line.find(' ', method_end + 1);
    if (path_end == std::string::npos)
        throw std::runtime_error("Malformed request line: missing path");

	method = line.substr(0, method_end);
	if (method != "GET" && method != "POST" && method != "DELETE")
		throw std::runtime_error(RED "HTTP method no soported: " YELLOW + method + WHITE);

	path = line.substr(method_end + 1, path_end - method_end - 1);
	version = line.substr(path_end + 1);

	if (version.empty() || version.compare(0, 5, "HTTP/") != 0)
        throw std::runtime_error("Invalid HTTP version: " + version);
}

void	HttpRequestManager::parseHeaders(std::istream &stream)
{
	std::string line;

	while (std::getline(stream, line) && line != "\r") 
	{
		size_t colon = line.find(':');
		if (colon == std::string::npos) 
		{
			throw std::runtime_error(RED "Header error: " YELLOW + line + "\n\t->" + GREEN" NO SEMICOLON" + WHITE);
		}

		std::string key = line.substr(0, colon);
		std::string value = line.substr(colon + 1);

		key.erase(key.find_last_not_of(" \r") + 1);
		value.erase(0, value.find_first_not_of(" "));
		value.erase(value.find_last_not_of("\r") + 1);

		if (!key.size())
			throw std::runtime_error(RED "Header error: " YELLOW + line + "\n\t->" + GREEN" KEY EMPTY" + WHITE);

		headers[to_lowercase(key)] = value;
	}
}

void	HttpRequestManager::parseBody(std::istream &stream)
{
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

HttpRequest *HttpRequestManager::buildHttpRequest()
{
	HttpRequest *request = new HttpRequest(method, path, version);
	request->handleContentLength(getHeader("content-length"));
	request->handleContentType(getHeader("content-type"));
	request->handleHost(getHeader("host"));
	request->handleConnection(getHeader("connection"));

	request->handleBody(body);

	return(request);
}