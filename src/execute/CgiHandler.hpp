#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include "HttpRequest.hpp"
# include "../builder/ServerConfig.hpp"
# include "ServerUtils.hpp"
# include <exception>
# include <iostream>
# include "CgiEnvBuilder.hpp"
# include <unistd.h>
# include "../utils/to_string.hpp"
# include "../utils/utils.hpp"
# include "EpollManager.hpp"
# include "CgiHeaderParser.hpp"

class CgiHandler {

	private:

		HttpRequest					*_req;
		ServerConfig				*_server;
		LocationConfig				*_location;
		std::string					_extension;
		std::string					_requestPath;
		std::string					_clientIp;
		int							_clientFd;

		int							_inputFd;
		int							_outputFd;
		bool						_inputFdClosed;
		bool						_outputFdClosed;
		std::string					_buffer;
		size_t						_bytesWritten;
		size_t						_bytesSent;

		std::string					_scriptPath;
		std::string					_interpreterPath;
		char						**_envv;

	public:

		CgiHandler(HttpRequest *req, std::string clientIp, std::vector<ServerConfig *> servers);

		bool	executeCgi(std::map<int, CgiHandler *> &cgiInputFd, std::map<int, CgiHandler *> &cgiOutputFd, int clientFd);
		bool	isCgiRequest(void);

		bool	checkScriptPermissions(void);
		bool	checkInterpreterPermissions(void);

		void	appendToCgiBuffer(char *buffer, ssize_t bytes_read);
		void	updateBytesWritten(size_t newBytesWritten);
		std::string	parseCgiBuffer(void);

		void	setInputAsClosed(void);
		void	setOutputAsClosed(void);

		int		getClientFd(void);
		int		getInputFd(void);
		int		getOutputFd(void);
		bool	getInputFdClosed(void);
		bool	getOutputFdClosed(void);
		size_t	getBytesWritten(void);
		size_t&	getBytesSent(void);
		std::string getBuffer(void);
		std::string getRequestBody(void);
		std::string	getGivenEnvvByKey(std::string key);
};

#endif