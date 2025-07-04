#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include "HttpRequest.hpp"
# include "../builder/ServerConfig.hpp"
# include "ServerUtils.hpp"
# include <exception>
# include <iostream>
# include "CgiEnvBuilder.hpp"
# include <unistd.h>

class CgiHandler {

	private:

		HttpRequest					*_req;
		ServerConfig				*_server;
		LocationConfig				*_location;
		std::string					_extension;
		std::string					_requestPath;
		std::string					_clientIp;

		const char					*_scriptPath;
		const char					*_interpreterPath;
		char						**_envv;

	public:

		CgiHandler(HttpRequest *req, std::string clientIp, std::vector<ServerConfig *> servers);

		bool	executeCgi(void);
		bool	isCgiRequest(void);
		void	resolveScriptPath(void);
		void	resolveInterpreterPath(void);
		void	buildEnv(void);
};

#endif