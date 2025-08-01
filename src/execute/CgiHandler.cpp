#include "CgiHandler.hpp"
#include <stdio.h>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include "ResponseFactory.hpp"

std::map<int, std::string> CgiHandler::error_codes;
std::map<std::string, std::string> CgiHandler::mime_types;
std::set<std::string> CgiHandler::known_mime_types;

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

CgiHandler::CgiHandler(HttpRequest *req, std::string clientIp, std::vector<ServerConfig *> servers)
	: _req(req), _clientIp(clientIp), _bytesWritten(0)
{
	this->_outputFdClosed = true;
	this->_inputFdClosed = true;
	if (!req)
		throw std::invalid_argument("HttpRequest pointer is null");
	if (servers.empty())
		throw std::invalid_argument("No servers found");

	this->_requestPath = req->getPath();

	this->_server = ServerUtils::getServerByHostName(req->getServerName(), servers);
	if (this->_server == NULL)
	{
		this->_server = servers[0];
		if (!this->_server)
			throw std::runtime_error("No matching server found for host: " + req->getServerName());
	}

	this->_location = ServerUtils::getLocationByRequestPath(this->_requestPath, this->_server);
	if (!this->_location)
	{
		throw std::runtime_error("No matching location found for host: " + this->_requestPath);
	}
}

CgiHandler::~CgiHandler()
{
}

/***********************************************************************/
/*                         Operator Overload                           */
/***********************************************************************/

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

bool	CgiHandler::isCgiRequest(void)
{
	if (!this->_location && this->_location->cgiDirective == false)
	{
		std::cerr << "Error with location in CGI\n";
		return (false);
	}

	std::string::size_type dotPos = this->_requestPath.rfind('.');
	if (dotPos == std::string::npos)
		return (false);

	this->_extension = this->_requestPath.substr(dotPos);
	for (size_t i = 0; i < this->_location->cgi.size(); i++)
	{
		if (this->_extension == this->_location->cgi[i]->extension)
			return (true);
	}
	return (false);
}

std::string CgiHandler::parseCgiBuffer()
{
    std::istringstream iss(this->_buffer);
    std::string line;
    std::string body;
    std::map<std::string, std::string> headerMap;
    bool headers_ended = false;

    while (std::getline(iss, line))
    {
        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);

        if (!headers_ended)
        {
            if (line.empty())
            {
                headers_ended = true;
                continue;
            }
            size_t pos = line.find(':');
            if (pos != std::string::npos)
            {
                std::string key = trimSpaces(line.substr(0, pos));
                std::string value = trimSpaces(line.substr(pos + 1));
                headerMap[key] = value;
            }
        }
        else
        {
            body += line + "\n";
        }
    }

    std::string statusLine;
    if (headerMap.count("Status"))
    {
        statusLine = "HTTP/1.1 " + headerMap["Status"] + "\r\n";
        headerMap.erase("Status");
    }
    else if (headerMap.count("Location"))
    {
        statusLine = "HTTP/1.1 302 Found\r\n";
    }
    else
    {
        statusLine = "HTTP/1.1 200 OK\r\n";
    }

    if (!headerMap.count("Content-Length"))
    {
        std::ostringstream oss;
        oss << body.size();
        headerMap["Content-Length"] = oss.str();
    }

    std::ostringstream headerStream;
    headerStream << statusLine;

    for (std::map<std::string, std::string>::iterator it = headerMap.begin(); it != headerMap.end(); ++it)
    {
        headerStream << it->first << ": " << it->second << "\r\n";
    }
    headerStream << "\r\n";

    return headerStream.str() + body;
}

bool	CgiHandler::checkScriptPermissions(void)
{
	struct stat s;
	std::string script = ServerUtils::resolveScriptPath(_req, _location);

	if (script.find(".cgi") == std::string::npos)
	{
		if (stat(script.c_str(), &s) != 0)
		{
			ResponseFactory factory;
			if (errno == ENOENT)
			{
				HttpResponse response = factory.generateErrorResponse(404, *_server, _location);
				std::vector<char> responseBuffer = ServerUtils::prepareResponseBuffer(response);
				ServerUtils::sendErrorResponse(this->_clientFd, responseBuffer);
				return (false);
			}
			else
			{
				HttpResponse response = factory.generateErrorResponse(500, *_server, _location);
				std::vector<char> responseBuffer = ServerUtils::prepareResponseBuffer(response);
				ServerUtils::sendErrorResponse(this->_clientFd, responseBuffer);
				return (false);
			}
		}
		int result = access(script.c_str(), R_OK);
		if (result == -1)
		{
			ResponseFactory factory;
			perror("access");
			HttpResponse response = factory.generateErrorResponse(403, *_server, _location);
			std::vector<char> responseBuffer = ServerUtils::prepareResponseBuffer(response);
			ServerUtils::sendErrorResponse(this->_clientFd, responseBuffer);
			return (false);
		}
	}
	return (true);
}

bool	CgiHandler::checkInterpreterPermissions(void)
{
	struct stat s;
	if (stat(this->_interpreterPath.c_str(), &s) != 0)
	{
		ResponseFactory factory;
		if (errno == ENOENT)
		{
			HttpResponse response = factory.generateErrorResponse(404, *_server, _location);
			std::vector<char> responseBuffer = ServerUtils::prepareResponseBuffer(response);
			ServerUtils::sendErrorResponse(this->_clientFd, responseBuffer);
			return (false);
		}
		else
		{
			HttpResponse response = factory.generateErrorResponse(404, *_server, _location);
			std::vector<char> responseBuffer = ServerUtils::prepareResponseBuffer(response);
			ServerUtils::sendErrorResponse(this->_clientFd, responseBuffer);
			return (false);
		}
	}
	int result = access(this->_interpreterPath.c_str(), X_OK);
	if (result == -1)
	{
		ResponseFactory factory;
		perror("access");
		HttpResponse response = factory.generateErrorResponse(403, *_server, _location);
		std::vector<char> responseBuffer = ServerUtils::prepareResponseBuffer(response);
		ServerUtils::sendErrorResponse(this->_clientFd, responseBuffer);
		return (false);
	}
	return (true);
}

bool	CgiHandler::executeCgi(std::map<int, CgiHandler *> &cgiInputFd, std::map<int, CgiHandler *> &cgiOutputFd, int clientFd)
{
	this->_scriptPath = ServerUtils::resolveScriptPath(const_cast<HttpRequest *>(this->_req), const_cast<LocationConfig *>(this->_location));
	this->_interpreterPath = ServerUtils::resolveInterpreterPath(this->_location, this->_extension);
	this->_clientFd = clientFd;

	if (!checkScriptPermissions() || !checkInterpreterPermissions())
		return (false);

	int	input_pipe[2];
	int output_pipe[2];

	pipe(input_pipe);
	pipe(output_pipe);

	pid_t pid = fork();
	if (pid == 0)
	{
		CgiEnvBuilder *envBuilder = new CgiEnvBuilder(this->_req, this->_server, this->_location, this->_clientIp);
		this->_envv = envBuilder->build();
		delete envBuilder;

		/*// DEBUG:
        std::cout << "---- CGI ENV ----\n";
        for (int i = 0; this->_envv[i]; ++i)
            std::cout << this->_envv[i] << std::endl;
        std::cout << "Interpreter: " << this->_interpreterPath << std::endl;
        std::cout << "Script: " << this->_scriptPath << std::endl;
        std::cout << "-----------------\n";
		std::string body = this->getRequestBody();
		std::cout << "---- CGI BODY ----\n" << body << "\n------------------\n";
		//DELETE THIS*/

        if (!_req->getBodyFilePath().empty() && _req->getBodyFilePath() != "error_413_payload_too_large")
        {
            std::string body_path_env = "UPLOADED_FILE_PATH=" + _req->getBodyFilePath();
            
            int env_count = 0;
            while (this->_envv[env_count] != NULL)
                env_count++;

            char** new_envv = new char*[env_count + 2];
            for (int i = 0; i < env_count; ++i)
            {
                new_envv[i] = this->_envv[i];
            }
            new_envv[env_count] = new char[body_path_env.length() + 1];
            strcpy(new_envv[env_count], body_path_env.c_str());
            new_envv[env_count + 1] = NULL;
            delete[] this->_envv;
            this->_envv = new_envv;
        }

		dup2(input_pipe[0], STDIN_FILENO);
		dup2(output_pipe[1], STDOUT_FILENO);

		close(input_pipe[1]);
		close(output_pipe[0]);

		if (this->_interpreterPath.empty() || this->_interpreterPath == this->_scriptPath)
		{
			char *argv[] = {
				const_cast<char*>(this->_scriptPath.c_str()),
				NULL
			};
			alarm(5);
			execve(argv[0], argv, this->_envv);
		}
		else
		{
			char *argv[] = {
				const_cast<char*>(this->_interpreterPath.c_str()),
				const_cast<char*>(this->_scriptPath.c_str()),
				NULL
			};
			alarm(5);
			execve(argv[0], argv, this->_envv);
		}
		std::cerr << "Falla\n";
		exit(1);
	}
	else
	{
		close(input_pipe[0]);
		close(output_pipe[1]);
		if (this->_req->getMethod() == "POST" && _req->getBodyFilePath().empty())
		{
			ServerUtils::setNotBlockingFd(input_pipe[1]);
			EpollManager::getInstance().addFd(input_pipe[1], EPOLLOUT);
			this->_inputFd = input_pipe[1];
			this->_inputFdClosed = false;
			cgiInputFd[input_pipe[1]] = this;
		}
		else
    	{
			close(input_pipe[1]);
			this->_inputFdClosed = true;
    	}

		ServerUtils::setNotBlockingFd(output_pipe[0]);
		EpollManager::getInstance().addFd(output_pipe[0], EPOLLIN);
		this->_outputFd = output_pipe[0];
		this->_outputFdClosed = false;
		cgiOutputFd[output_pipe[0]] = this;
		return (true);
	}
	ResponseFactory factory;
	factory.generateErrorResponse(500, *_server, _location);
	return (false);
}

void	CgiHandler::appendToCgiBuffer(char *buffer, ssize_t bytes_read)
{
	this->_buffer.append(buffer, bytes_read);
}

void	CgiHandler::updateBytesWritten(size_t newBytesWritten)
{
	size_t	actualBytes = this->_bytesWritten;

	this->_bytesWritten = actualBytes + newBytesWritten;
}

void	CgiHandler::setInputAsClosed(void)
{
	if (this->_inputFdClosed == false)
		this->_inputFdClosed = true;
}

void	CgiHandler::setOutputAsClosed(void)
{
	if (this->_outputFdClosed == false)
		this->_outputFdClosed = true;
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/

int	CgiHandler::getClientFd(void)
{
	return (this->_clientFd);
}

int	CgiHandler::getInputFd(void)
{
	return (this->_inputFd);
}

int CgiHandler::getOutputFd(void)
{
	return (this->_outputFd);
}

size_t CgiHandler::getBytesWritten(void)
{
	return (this->_bytesWritten);
}

size_t& CgiHandler::getBytesSent(void)
{
	return (this->_bytesSent);
}

std::string CgiHandler::getBuffer(void)
{
	return (this->_buffer);
}

std::string CgiHandler::getRequestBody(void)
{
	if (!_req->getBodyFilePath().empty())
	{
		std::ifstream file(_req->getBodyFilePath().c_str(), std::ios::binary);
		if (file)
		{
			std::ostringstream ss;
			ss << file.rdbuf();
			file.close();
			//std::remove(_req->getBodyFilePath().c_str());
			return ss.str();
		}
		else
		{
			std::cerr << "CGI Error: Could not open temporary body file: " << _req->getBodyFilePath() << std::endl;
			return "";
		}
	}
	return (to_string(this->_req->getBody()));
}

bool	CgiHandler::getInputFdClosed(void)
{
	return (this->_inputFdClosed);
}

bool	CgiHandler::getOutputFdClosed(void)
{
	return (this->_outputFdClosed);
}

std::string	CgiHandler::getGivenEnvvByKey(std::string key)
{
	if (!this->_envv || !this->_envv[0])
		return (NULL);
	for (int i = 0; this->_envv[i]; i++)
    {
		std::string envvar = to_string(this->_envv[i]);
		size_t pos = envvar.find(key);
		if (pos != std::string::npos)
		{
			pos = envvar.find("=");
			std::string value = envvar.substr(pos + 1, envvar.size() - 1);
			return (value);
		}
	}
	return (NULL);
}

HttpRequest* CgiHandler::getRequest(void) const
{
	return this->_req;
}

void CgiHandler::loadErrorCodes()
{
	error_codes[100] = "Continue";
	error_codes[101] = "Switching Protocols";
	error_codes[102] = "Processing";
	error_codes[200] = "OK";
	error_codes[201] = "Created";
	error_codes[202] = "Accepted";
	error_codes[203] = "Non-Authoritative Information";
	error_codes[204] = "No Content";
	error_codes[205] = "Reset Content";
	error_codes[206] = "Partial Content";
	error_codes[207] = "Multi-Status";
	error_codes[208] = "Already Reported";
	error_codes[226] = "IM Used";
	error_codes[300] = "Multiple Choices";
	error_codes[301] = "Moved Permanently";
	error_codes[302] = "Found";
	error_codes[303] = "See Other";
	error_codes[304] = "Not Modified";
	error_codes[305] = "Use Proxy";
	error_codes[306] = "(Unused)";
	error_codes[307] = "Temporary Redirect";
	error_codes[308] = "Permanent Redirect";
	error_codes[400] = "Bad Request";
	error_codes[401] = "Unauthorized";
	error_codes[402] = "Payment Required";
	error_codes[403] = "Forbidden";
	error_codes[404] = "Not Found";
	error_codes[405] = "Method Not Allowed";
	error_codes[406] = "Not Acceptable";
	error_codes[407] = "Proxy Authentication Required";
	error_codes[408] = "Request Timeout";
	error_codes[409] = "Conflict";
	error_codes[410] = "Gone";
	error_codes[411] = "Length Required";
	error_codes[412] = "Precondition Failed";
	error_codes[413] = "Payload Too Large";
	error_codes[414] = "URI Too Long";
	error_codes[415] = "Unsupported Media Type";
	error_codes[416] = "Range Not Satisfiable";
	error_codes[417] = "Expectation Failed";
	error_codes[418] = "I'm a teapot";
	error_codes[421] = "Misdirected Request";
	error_codes[422] = "Unprocessable Entity";
	error_codes[423] = "Locked";
	error_codes[424] = "Failed Dependency";
	error_codes[425] = "Too Early";
	error_codes[426] = "Upgrade Required";
	error_codes[428] = "Precondition Required";
	error_codes[429] = "Too Many Requests";
	error_codes[431] = "Request Header Fields Too Large";
	error_codes[451] = "Unavailable For Legal Reasons";
	error_codes[500] = "Internal Server Error";
	error_codes[501] = "Not Implemented";
	error_codes[502] = "Bad Gateway";
	error_codes[503] = "Service Unavailable";
	error_codes[504] = "Gateway Timeout";
	error_codes[505] = "HTTP Version Not Supported";
}

void CgiHandler::loadMimeTypes()
{
    mime_types["html"]		= "text/html";
	mime_types["htm"]		= "text/html";
	mime_types["shtml"]		= "text/html";
	mime_types["css"]		= "text/css";
	mime_types["xml"]		= "text/xml";
	mime_types["gif"]		= "image/gif";
	mime_types["jpeg"]		= "image/jpeg";
	mime_types["jpg"]		= "image/jpeg";
	mime_types["js"]		= "application/javascript";
	mime_types["atom"]		= "application/atom+xml";
	mime_types["rss"]		= "application/rss+xml";
	mime_types["mml"]		= "text/mathml";
	mime_types["txt"]		= "text/plain";
	mime_types["ini"]		= "text/plain";
	mime_types["cfg"]		= "text/plain";
	mime_types["jad"]		= "text/vnd.sun.j2me.app-descriptor";
	mime_types["wml"]		= "text/vnd.wap.wml";
	mime_types["htc"]		= "text/x-component";
	mime_types["png"]		= "image/png";
	mime_types["tif"]		= "image/tiff";
	mime_types["tiff"]		= "image/tiff";
	mime_types["wbmp"]		= "image/vnd.wap.wbmp";
	mime_types["ico"]		= "image/x-icon";
	mime_types["jng"]		= "image/x-jng";
	mime_types["bmp"]		= "image/x-ms-bmp";
	mime_types["svg"]		= "image/svg+xml";
	mime_types["svgz"]		= "image/svg+xml";
	mime_types["webp"]		= "image/webp";
	mime_types["woff"]		= "application/font-woff";
	mime_types["jar"]		= "application/java-archive";
	mime_types["war"]		= "application/java-archive";
	mime_types["ear"]		= "application/java-archive";
	mime_types["json"]		= "application/json";
	mime_types["hqx"]		= "application/mac-binhex40";
	mime_types["doc"]		= "application/msword";
	mime_types["pdf"]		= "application/pdf";
	mime_types["ps"]		= "application/postscript";
	mime_types["eps"]		= "application/postscript";
	mime_types["ai"]		= "application/postscript";
	mime_types["rtf"]		= "application/rtf";
	mime_types["m3u8"]		= "application/vnd.apple.mpegurl";
	mime_types["xls"]		= "application/vnd.ms-excel";
	mime_types["eot"]		= "application/vnd.ms-fontobject";
	mime_types["ppt"]		= "application/vnd.ms-powerpoint";
	mime_types["wmlc"]		= "application/vnd.wap.wmlc";
	mime_types["kml"]		= "application/vnd.google-earth.kml+xml";
	mime_types["kmz"]		= "application/vnd.google-earth.kmz";
	mime_types["7z"]		= "application/x-7z-compressed";
	mime_types["cco"]		= "application/x-cocoa";
	mime_types["jardiff"]	= "application/x-java-archive-diff";
	mime_types["jnlp"]		= "application/x-java-jnlp-file";
	mime_types["run"]		= "application/x-makeself";
	mime_types["pl"]		= "application/x-perl";
	mime_types["pm"]		= "application/x-perl";
	mime_types["prc"]		= "application/x-pilot";
	mime_types["pdb"]		= "application/x-pilot";
	mime_types["rar"]		= "application/x-rar-compressed";
	mime_types["rpm"]		= "application/x-redhat-package-manager";
	mime_types["sea"]		= "application/x-sea";
	mime_types["swf"]		= "application/x-shockwave-flash";
	mime_types["sit"]		= "application/x-stuffit";
	mime_types["tcl"]		= "application/x-tcl";
	mime_types["tk"]		= "application/x-tcl";
	mime_types["der"]		= "application/x-x509-ca-cert";
	mime_types["pem"]		= "application/x-x509-ca-cert";
	mime_types["crt"]		= "application/x-x509-ca-cert";
	mime_types["xpi"]		= "application/x-xpinstall";
	mime_types["xhtml"]		= "application/xhtml+xml";
	mime_types["xspf"]		= "application/xspf+xml";
	mime_types["zip"]		= "application/zip";
	mime_types["bin"]		= "application/octet-stream";
	mime_types["exe"]		= "application/octet-stream";
	mime_types["dll"]		= "application/octet-stream";
	mime_types["deb"]		= "application/octet-stream";
	mime_types["dmg"]		= "application/octet-stream";
	mime_types["iso"]		= "application/octet-stream";
	mime_types["img"]		= "application/octet-stream";
	mime_types["msi"]		= "application/octet-stream";
	mime_types["msp"]		= "application/octet-stream";
	mime_types["msm"]		= "application/octet-stream";
	mime_types["docx"]		= "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	mime_types["xlsx"]		= "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	mime_types["pptx"]		= "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	mime_types["mid"]		= "audio/midi";
	mime_types["midi"]		= "audio/midi";
	mime_types["kar"]		= "audio/midi";
	mime_types["mp3"]		= "audio/mpeg";
	mime_types["ogg"]		= "audio/ogg";
	mime_types["m4a"]		= "audio/x-m4a";
	mime_types["ra"]		= "audio/x-realaudio";
	mime_types["3gpp"]		= "video/3gpp";
	mime_types["3gp"]		= "video/3gpp";
	mime_types["ts"]		= "video/mp2t";
	mime_types["mp4"]		= "video/mp4";
	mime_types["mpeg"]		= "video/mpeg";
	mime_types["mpg"]		= "video/mpeg";
	mime_types["mov"]		= "video/quicktime";
	mime_types["webm"]		= "video/webm";
	mime_types["flv"]		= "video/x-flv";
	mime_types["m4v"]		= "video/x-m4v";
	mime_types["mng"]		= "video/x-mng";
	mime_types["asx"]		= "video/x-ms-asf";
	mime_types["asf"]		= "video/x-ms-asf";
	mime_types["wmv"]		= "video/x-ms-wmv";
	mime_types["avi"]		= "video/x-msvideo";

	known_mime_types.insert("text/html");
	known_mime_types.insert("text/css");
	known_mime_types.insert("text/xml");
	known_mime_types.insert("image/gif");
	known_mime_types.insert("image/jpeg");
	known_mime_types.insert("application/javascript");
	known_mime_types.insert("application/atom+xml");
	known_mime_types.insert("application/rss+xml");
	known_mime_types.insert("text/mathml");
	known_mime_types.insert("text/plain");
	known_mime_types.insert("text/vnd.sun.j2me.app-descriptor");
	known_mime_types.insert("text/vnd.wap.wml");
	known_mime_types.insert("text/x-component");
	known_mime_types.insert("image/png");
	known_mime_types.insert("image/tiff");
	known_mime_types.insert("image/vnd.wap.wbmp");
	known_mime_types.insert("image/x-icon");
	known_mime_types.insert("image/x-jng");
	known_mime_types.insert("image/x-ms-bmp");
	known_mime_types.insert("image/svg+xml");
	known_mime_types.insert("image/webp");
	known_mime_types.insert("application/font-woff");
	known_mime_types.insert("application/java-archive");
	known_mime_types.insert("application/json");
	known_mime_types.insert("application/mac-binhex40");
	known_mime_types.insert("application/msword");
	known_mime_types.insert("application/pdf");
	known_mime_types.insert("application/postscript");
	known_mime_types.insert("application/rtf");
	known_mime_types.insert("application/vnd.apple.mpegurl");
	known_mime_types.insert("application/vnd.ms-excel");
	known_mime_types.insert("application/vnd.ms-fontobject");
	known_mime_types.insert("application/vnd.ms-powerpoint");
	known_mime_types.insert("application/vnd.wap.wmlc");
	known_mime_types.insert("application/vnd.google-earth.kml+xml");
	known_mime_types.insert("application/vnd.google-earth.kmz");
	known_mime_types.insert("application/x-7z-compressed");
	known_mime_types.insert("application/x-cocoa");
	known_mime_types.insert("application/x-java-archive-diff");
	known_mime_types.insert("application/x-java-jnlp-file");
	known_mime_types.insert("application/x-makeself");
	known_mime_types.insert("application/x-perl");
	known_mime_types.insert("application/x-pilot");
	known_mime_types.insert("application/x-rar-compressed");
	known_mime_types.insert("application/x-redhat-package-manager");
	known_mime_types.insert("application/x-sea");
	known_mime_types.insert("application/x-shockwave-flash");
	known_mime_types.insert("application/x-stuffit");
	known_mime_types.insert("application/x-tcl");
	known_mime_types.insert("application/x-x509-ca-cert");
	known_mime_types.insert("application/x-www-form-urlencoded");
	known_mime_types.insert("application/x-xpinstall");
	known_mime_types.insert("application/xhtml+xml");
	known_mime_types.insert("application/xspf+xml");
	known_mime_types.insert("application/zip");
	known_mime_types.insert("application/octet-stream");
	known_mime_types.insert("application/vnd.openxmlformats-officedocument.wordprocessingml.document");
	known_mime_types.insert("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
	known_mime_types.insert("application/vnd.openxmlformats-officedocument.presentationml.presentation");
	known_mime_types.insert("audio/midi");
	known_mime_types.insert("audio/mpeg");
	known_mime_types.insert("audio/ogg");
	known_mime_types.insert("audio/x-m4a");
	known_mime_types.insert("audio/x-realaudio");
	known_mime_types.insert("video/3gpp");
	known_mime_types.insert("video/mp2t");
	known_mime_types.insert("video/mp4");
	known_mime_types.insert("video/mpeg");
	known_mime_types.insert("video/quicktime");
	known_mime_types.insert("video/webm");
	known_mime_types.insert("video/x-flv");
	known_mime_types.insert("video/x-m4v");
	known_mime_types.insert("video/x-mng");
	known_mime_types.insert("video/x-ms-asf");
	known_mime_types.insert("video/x-ms-wmv");
	known_mime_types.insert("video/x-msvideo");
}

/***********************************************************************/
/*                          Private Functions                          */
/***********************************************************************/