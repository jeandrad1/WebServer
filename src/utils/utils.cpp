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

/*void printLocation(LocationConfig *location, int indent = 0)
{
    std::string spaces(indent * 2, ' ');
    std::cout << spaces << "Location:" << std::endl;
    std::cout << spaces << "  Root: " << location->root << std::endl;
    std::cout << spaces << "  Index: ";
    for (size_t i = 0; i < location->index.size(); ++i)
        std::cout << location->index[i] << " ";
    std::cout << std::endl;
    std::cout << spaces << "  Autoindex: " << (location->autoindex ? "on" : "off") << std::endl;
    std::cout << spaces << "  Client Max Body Size: " << location->clientMaxBodySize << std::endl;
    std::cout << spaces << "  Return Code: " << location->_return->code << std::endl;
    std::cout << spaces << "  Return HTTP: " << location->_return->http << std::endl;
}

void printServer(ServerConfig *server, int indent = 0)
{
    std::string spaces(indent * 2, ' ');
    std::cout << spaces << "Server:" << std::endl;
    
    for (size_t i = 0; i < server->listen.size(); ++i)
    {
        std::cout << spaces << "  Listen IP: " << server->listen[i]->ip << std::endl;
        std::cout << spaces << "  Listen Port: " << server->listen[i]->port << std::endl;
    }

    if (!server->serverNames.empty())
        std::cout << spaces << "  Server Name: " << server->serverNames[0] << std::endl;
    else
        std::cout << spaces << "  Server Name: (none)" << std::endl;

    std::cout << spaces << "  Server Name: " << server->serverNames[0] << std::endl;
    std::cout << spaces << "  Root: " << server->root << std::endl;
    std::cout << spaces << "  Index: ";
    for (size_t i = 0; i < server->index.size(); ++i)
        std::cout << server->index[i] << " ";
    std::cout << std::endl;
    std::cout << spaces << "  Autoindex: " << (server->autoindex ? "on" : "off") << std::endl;
    std::cout << spaces << "  Client Max Body Size: " << server->clientMaxBodySize << std::endl;
    std::cout << spaces << "  Return Code: " << server->_return->code << std::endl;
    std::cout << spaces << "  Return HTTP: " << server->_return->http << std::endl;

    for (size_t i = 0; i < server->locations.size(); ++i)
    {
        printLocation(server->locations[i], indent + 1);
    }
}

void printHttpConfig(HttpConfig *httpConfig, int indent = 0)
{
    std::string spaces(indent * 2, ' ');
    std::cout << spaces << "HttpConfig:" << std::endl;
    std::cout << spaces << "  Client Max Body Size: " << httpConfig->clientMaxBodySize << std::endl;

    for (size_t i = 0; i < httpConfig->servers.size(); ++i)
    {
        printServer(httpConfig->servers[i], indent + 1);
    }
}
*/
void printBuiltConfigs(const std::vector<IConfig *> &builtConfigs)
{
    std::vector<IConfig * >::const_iterator ite = builtConfigs.end();
    
    for (std::vector<IConfig * >::const_iterator it = builtConfigs.begin(); it != ite; it++)
    {
        std::cout<<"Entra"<<std::endl;
        if (ServerConfig* server = static_cast<ServerConfig*>(*it))
        {
            std::cout<<"Entra en server"<<std::endl;
            server->ServerConfig::printValues();
            std::cout<<"Sale"<<std::endl;
        }
        else if (HttpConfig* http = static_cast<HttpConfig*>(*it))
        {
            std::cout<<"Entra en server"<<std::endl;
            http->HttpConfig::printValues();
            std::cout<<"Sale"<<std::endl;
        }
        else if (LocationConfig* location = static_cast<LocationConfig*>(*it))
        {
            std::cout<<"Entra en server"<<std::endl;
            location->LocationConfig::printValues();
            std::cout<<"Sale"<<std::endl;
        }

        else
            std::cout<<"No entra"<<std::endl;
    }
}