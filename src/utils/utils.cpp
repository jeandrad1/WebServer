#include <string>
#include <iostream>
#include <vector>
#include <sstream>


bool	isInteger(std::string value)
{
	if (value.empty())
		return (false);

	for (size_t i = 0; i < value.size(); ++i)
	{
		if (!isdigit(value[i]))
			return (false);
	}
	return (true);
}

bool	isURI(const std::string &path)
{
	if (path[0] == '/')
    	return (true);
	return (false);
}

bool	isURL(const std::string &path)
{
	if (path.find("http://") == 0 || path.find("https://") == 0)
		return (true);
	return (false);
}

bool	checkValidErrorCode(int code)
{
	if (code >= 400 && code <= 599)
		return (true);
	return (false);
}

bool	checkCodeIsValid(int code)
{
	if (code >= 100 && code <= 599)
		return (true);
	return (false);
}

std::string	getSubstringBeforeSemicolon(const std::string &value)
{
	if (value.empty() || value[value.size() - 1] != ';')
		return "";

	return value.substr(0, value.size() - 1);
}

bool	checkChars(const std::string &value)
{
	for (size_t i = 0; i < value.size(); i++)
	{
		char c = value[i];
		if (c > 31 && c < 127)
			return true;
	}
	return false;
}

std::string	trim(const std::string &str)
{
	if (str.size() == 0)
		return "";

	size_t	first = str.find_first_not_of(" \t\r\n");
	if (first == std::string::npos)
		return "";

	size_t	last = str.find_last_not_of(" \t\r\n");
	return str.substr(first, last - first + 1);
}

int	countWhitespace(const std::string &str)
{
	int	count = 0;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (std::isspace(str[i]))
			count++;
	}
	return count;
}

bool	checkFilename(const std::string &word)
{
	return word.size() >= 5 && word.substr(word.size() - 5) == ".html";
}

std::vector<std::string> split_str(const std::string &str, const std::string &delimiter)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos)
    {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    tokens.push_back(str.substr(start));
    return tokens;
}

//Printers
#include <iostream>
#include "../builder/HttpBuilder.hpp"
#include "../builder/ServerConfig.hpp"
#include "../builder/LocationConfig.hpp"

void printLocationConfig(LocationConfig *locationConfig, int indent = 0)
{
    std::string spaces(indent * 2, ' ');
    std::cout << spaces << "LocationConfig:" << std::endl;
    std::cout << spaces << "  Root: " << locationConfig->root << std::endl;
    std::cout << spaces << "  Index: ";
    for (size_t i = 0; i < locationConfig->index.size(); ++i)
        std::cout << locationConfig->index[i] << " ";
    std::cout << std::endl;
    std::cout << spaces << "  Autoindex: " << (*locationConfig->autoindex ? "on" : "off") << std::endl;
    std::cout << spaces << "  Client Max Body Size: " << locationConfig->clientmaxbodysize << std::endl;
    std::cout << spaces << "  Return Code: " << locationConfig->_return->code << std::endl;
    std::cout << spaces << "  Return HTTP: " << locationConfig->_return->http << std::endl;
}

void printServerConfig(ServerConfig *serverConfig, int indent = 0)
{
    std::string spaces(indent * 2, ' ');
    std::cout << spaces << "ServerConfig:" << std::endl;
    std::cout << spaces << "  Listen IP: " << serverConfig->listen->ip << std::endl;
    std::cout << spaces << "  Listen Port: " << serverConfig->listen->port << std::endl;
    std::cout << spaces << "  Server Name: " << serverConfig->serverName << std::endl;
    std::cout << spaces << "  Root: " << serverConfig->root << std::endl;
    std::cout << spaces << "  Index: ";
    for (size_t i = 0; i < serverConfig->index.size(); ++i)
        std::cout << serverConfig->index[i] << " ";
    std::cout << std::endl;
    std::cout << spaces << "  Autoindex: " << (*serverConfig->autoindex ? "on" : "off") << std::endl;
    std::cout << spaces << "  Client Max Body Size: " << serverConfig->clientmaxbodysize << std::endl;
    std::cout << spaces << "  Return Code: " << serverConfig->_return->code << std::endl;
    std::cout << spaces << "  Return HTTP: " << serverConfig->_return->http << std::endl;

    for (size_t i = 0; i < serverConfig->locations.size(); ++i)
    {
        printLocationConfig(serverConfig->locations[i], indent + 1);
    }
}

void printHttpConfig(HttpConfig *httpConfig, int indent = 0)
{
    std::string spaces(indent * 2, ' ');
    std::cout << spaces << "HttpConfig:" << std::endl;
    std::cout << spaces << "  Client Max Body Size: " << httpConfig->clientMaxBodySize << std::endl;

    for (size_t i = 0; i < httpConfig->servers.size(); ++i)
    {
        printServerConfig(httpConfig->servers[i], indent + 1);
    }
}

void printBuiltConfigs(const std::vector<void *> &builtConfigs)
{
    for (size_t i = 0; i < builtConfigs.size(); ++i)
    {
        HttpConfig *httpConfig = dynamic_cast<HttpConfig *>(static_cast<HttpConfig *>(builtConfigs[i]));
        if (httpConfig)
        {
            printHttpConfig(httpConfig);
            continue;
        }

        ServerConfig *serverConfig = dynamic_cast<ServerConfig *>(static_cast<ServerConfig *>(builtConfigs[i]));
        if (serverConfig)
        {
            printServerConfig(serverConfig);
            continue;
        }

        LocationConfig *locationConfig = dynamic_cast<LocationConfig *>(static_cast<LocationConfig *>(builtConfigs[i]));
        if (locationConfig)
        {
            printLocationConfig(locationConfig);
            continue;
        }

        std::cout << "Unknown configuration type." << std::endl;
    }
}