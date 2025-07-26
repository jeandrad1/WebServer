#include "HttpRequest.hpp"

//GET, POST y DELETE

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/


HttpRequest::HttpRequest(std::string src_method, std::string src_path, std::string src_version)
{
	method = src_method; path = src_path; version = src_version;
}

HttpRequest::~HttpRequest(){}

/***********************************************************************/
/*                         Operator Overload                           */
/***********************************************************************/

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

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

void	HttpRequest::handleContentLength(std::string content_length_str)
{
	if (!content_length_str.empty()) 
	{
		safe_atoll(content_length_str, contentLength);
		this->contentLengthString = content_length_str;
	}
	else{
		contentLength = 0;
	}
}

void HttpRequest::handleTransferEncoding(std::string transfer_encoding_str)
{
	if (transfer_encoding_str == "chunked")
		transferEncoding = transfer_encoding_str;
	else
		transferEncoding = "unchunked";
}

void	HttpRequest::handleQueryString(std::string query_string_str)
{
	query_string = query_string_str;
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

void HttpRequest::handleHost(const std::string &host_str)
{
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
}

void HttpRequest::handleConnection(std::string connection_str)
{
	if (connection_str.empty() || to_lowercase(connection_str) == "keep-alive")
		connection = true;
	else if (to_lowercase(connection_str) == "close")
		connection = false;
	else
		throw std::runtime_error(RED "Header error: " YELLOW "VALUE NOT CORRECT TO CONNECTION" WHITE);
}

void HttpRequest::handleUserAgent(std::string userAgent_str)
{
	this->userAgent = userAgent_str;
}

void HttpRequest::handleAccept(std::string accept_str)
{
	this->accept = accept_str;
}

void HttpRequest::handleBody(std::string str_body)
{
	if (str_body.empty())
		return;
 	if (transferEncoding != "chunked")
	{
		for(size_t i = 0; i < str_body.size(); i++)
			body.push_back(str_body[i]);
	}
	else
	{
		std::string chunked_body = chunkedBodyManager(str_body);
		for(size_t i = 0; i < chunked_body.size(); i++)
			body.push_back(chunked_body[i]);
	}
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/


std::string	HttpRequest::getMethod() const
{
	return(method);
}

std::string	HttpRequest::getPath() const
{
	return(path);
}

std::string HttpRequest::getQueryString() const
{
	return (query_string);
}

std::string	HttpRequest::getVersion() const
{
	return(version);
}

std::string HttpRequest::getUserAgent() const
{
	return (userAgent);
}

std::string HttpRequest::getAccept() const
{
	return (accept);
}

std::string HttpRequest::getHost() const
{
	return (host);
}

std::string HttpRequest::getServerName() const
{
	return (serverName);
}

std::string HttpRequest::getServerPort() const
{
	return (serverPort);
}

long long HttpRequest::getContentLength()
{
	return(contentLength);
}

std::string HttpRequest::getTransferEncoding() const
{
	return(transferEncoding);
}

std::string HttpRequest::getContentType() const
{
	return (contentType);
}

std::string HttpRequest::getContentLengthString() const
{
	return(contentLengthString);
}

std::vector<unsigned char> HttpRequest::getBody() const
{
	return(body);
}

std::string HttpRequest::getHeader(const std::string& key) const
{
	std::map<std::string, std::string>::const_iterator it = headers.find(key);
	if (it != headers.end())
		return it->second;
	return "";
}

/***********************************************************************/
/*                          Private Functions                          */
/***********************************************************************/

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
	if (body.empty())
		std::cout << RED "\nNO BODY\n" WHITE;
	else
	{
		std::cout << GREEN "\nBODY:\n" WHITE;
		std::vector<unsigned char>::iterator it = body.begin();
		for(std::vector<unsigned char>::iterator ite = body.end(); it != ite; it++)
		{
			std::cout << (*it);
		}
		std::cout << CYAN "\nEND BODY\n" WHITE;
	}
}

std::string HttpRequest::chunkedBodyManager(const std::string &str_body)
{
	std::string finalBody;
	size_t pos = 0;

	while (true)
	{
		size_t size_end = str_body.find("\r\n", pos);
		if (size_end == std::string::npos)
			break;

		std::string size_str = str_body.substr(pos, size_end - pos);

		long long chunk_size = 0;
		try 
		{
			chunk_size = hexToDecimal(size_str);
		} 
		catch (...)
		{
			break;
		}

		if (chunk_size == 0)
			break;

		pos = size_end + 2; // Skip "\r\n"

		if (pos + chunk_size > str_body.size())
			break;

		finalBody.append(str_body.substr(pos, chunk_size));

		pos += chunk_size;

		// Skip \r\n
		if (str_body.substr(pos, 2) != "\r\n")
			break;

		pos += 2;
	}

	return finalBody;
}
