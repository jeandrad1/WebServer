#include "HttpRequest.hpp"

//GET, POST y DELETE

HttpRequest::HttpRequest(std::string src_method, std::string src_path, std::string src_version)
{
	method = src_method; path = src_path; version = src_version;
}

HttpRequest::~HttpRequest(){}

void	HttpRequest::handleContentLength(std::string content_length_str)
{
	if (!content_length_str.empty()) 
	{
		safe_atoll(content_length_str, contentLength);
	}
	else{
		contentLength = 0;
	}
}

static bool	checkType(std::string content_type_str)
{
	std::vector<std::string> valid_types;
	valid_types.push_back("text/plain");
	valid_types.push_back("text/html");
	valid_types.push_back("text/css");
	valid_types.push_back("application/json");
	valid_types.push_back("application/xml");
	valid_types.push_back("application/x-www-form-urlencoded");
	valid_types.push_back("multipart/form-data");
	valid_types.push_back("image/jpeg");
	valid_types.push_back("image/png");
	valid_types.push_back("application/octet-stream");

	for (size_t i = 0; i < valid_types.size(); i++)
	{
		if (content_type_str == valid_types[i])
			return true;
	}
	return(false);
}

void	HttpRequest::handleContentType(std::string content_type_str)
{
	if (checkType(content_type_str))
		contentType = content_type_str;
	else if (content_type_str.empty())
		contentType = "EMPTY";
	else
		throw std::runtime_error(RED "Header error: " YELLOW "VALUE NOT CORRECT TO CONTENT-TYPE" WHITE);
}

void	HttpRequest::handleHost(std::string host_str)
{
	if (!host_str.empty())
		host = host_str;
	else
		host = "EMPTY";
}

void HttpRequest::handleConnection(std::string connection_str)
{
	if (!connection_str.empty() || to_lowercase(connection_str) == "keep-alive")
		connection = true;
	else if (to_lowercase(connection_str) == "close")
		connection = false;
	else
		throw std::runtime_error(RED "Header error: " YELLOW "VALUE NOT CORRECT TO CONNECTION" WHITE);
}

void HttpRequest::handleBody(std::string str_body)
{
	if (!str_body.empty())
		return;
	for(size_t i = 0; i < str_body.size(); i++)
		body.push_back(str_body[i]);
}

/////////////////////////////////////////////////////////
/////////////////////GETTERS/////////////////////////////
/////////////////////////////////////////////////////////

std::string	HttpRequest::getMethod() const
{
	return(method);
}

std::string	HttpRequest::getPath() const
{
	return(path);
}

std::string	HttpRequest::getVersion() const
{
	return(version);
}

long long HttpRequest::getContentLenght()
{
	return(contentLength);
}

std::vector<unsigned char> HttpRequest::getBody() const
{
	return(body);
}

/////////////////////////////////////////////////////////
/////////////////////PRINTER/////////////////////////////
/////////////////////////////////////////////////////////

void HttpRequest::HttpRequestPrinter()
{
	std::cout << "method: " << method << "\n";
    std::cout << "path: " << path << "\n";
    std::cout << "version: " << version << "\n\n";
	std::cout << " - > contentLength " << contentLength << "\n";
	std::cout << " - > contentType " << contentType << "\n";
	std::cout << " - > host " << host << "\n";
	std::cout << " - > connection ";
	if (connection == true)
		std::cout << "OPEN\n";
	else if (connection == false)
		std::cout << "CLOSE\n";
	if (!body.empty())
		std::cout << "\nNO BODY\n";
	else
	{
		std::cout << "\nBODY:\n";
		std::vector<unsigned char>::iterator it = body.begin();
		for(std::vector<unsigned char>::iterator ite = body.end(); it != ite; it++)
		{
			std::cout << (*it) << "\n";
		}
	}
}