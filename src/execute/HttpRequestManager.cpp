#include "HttpRequestManager.hpp"
#include <cstdio>
#include "ServerUtils.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

/***********************************************************************/
/*                         Operator Overload                           */
/***********************************************************************/

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/

/***********************************************************************/
/*                          Private Functions                          */
/***********************************************************************/

std::string HttpRequestManager::getHeader(const std::string& key) const 
{
	std::map<std::string, std::string>::const_iterator it = headers.find(key);
	if (it != headers.end())
	{
		return it->second;
	}
	return "";
}

/*void	HttpRequestManager::parseHttpHeader(const std::string& raw_header)
{
	std::istringstream stream(raw_header);
	std::string line;

	std::getline(stream, line);
	
	parseRequestLine(line);
	parseHeaders(stream);
}*/

void HttpRequestManager::parseHttpRequest(const std::string& raw_request, std::vector<ServerConfig *> servers)
{
	std::istringstream stream(raw_request);
	std::string line;

	std::getline(stream, line);
	
	parseRequestLine(line);

	parseHeaders(stream);
	if (!checkClientMaxBodySize(servers))
	{
		throw std::out_of_range("ClientMaxBodySize too large");
	}
	parseBody(stream);
}

bool HttpRequestManager::checkClientMaxBodySize(std::vector<ServerConfig *> servers)
{
	if (method == "POST")
	{
		std::istringstream iss(getHeader("content-length"));
		long long contentLength = 0;
		iss >> contentLength;
		std::string host_str = getHeader("host");
	
		std::string serverName;
		std::string serverPort;
		std::string host;
		if (!host_str.empty())
		{
			host = host_str;
			size_t colonPos = host_str.find(':');
			if (colonPos != std::string::npos)
			{
				serverName = host_str.substr(0, colonPos);
				serverPort = host_str.substr(colonPos + 1);
			}
			else
			{
				serverName = host_str;
				serverPort = "80"; // Default HTTP port if not specified
			}
		}
		else
		{
			host = "EMPTY";
			serverName = "localhost";
			serverPort = "80";
		}
	
		ServerConfig *server = ServerUtils::getServerByHostName(serverName, servers);
		if (server != NULL)
		{
			LocationConfig *location = ServerUtils::getLocationByRequestPath(path, server);
			if (location != NULL)
			{
				if (contentLength > location->getClientMaxBodySize())
				{
					std::cout << "Error: Body too large\n";
					return (false);
				}
				return (true);
			}
			if (contentLength > location->getClientMaxBodySize())
			{
				std::cout << "Error: Body too large\n";
				return (false);
			}
			return (true);
		}
		return (true);
	}
	return (true);
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

	size_t query_string_pos = path.find('?');
	if (query_string_pos != std::string::npos)
	{
		query_string = path.substr(query_string_pos + 1, path_end);
		path = path.substr(0, query_string_pos - 1);
	}

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
	request->handleQueryString(query_string);
	request->handleContentLength(getHeader("content-length"));
	request->handleTransferEncoding(getHeader("transfer-encoding"));
	request->handleContentType(getHeader("content-type"));
	request->handleHost(getHeader("host"));
	request->handleConnection(getHeader("connection"));
	request->handleUserAgent(getHeader("user-agent"));
	request->handleAccept(getHeader("accept"));
	request->handleBody(body);
    request->setHeaders(headers); 
	return(request);
}