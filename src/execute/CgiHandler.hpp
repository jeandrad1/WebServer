#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include "HttpRequest.hpp"
# include "../builder/ServerConfig.hpp"
# include "ServerUtils.hpp"
# include <exception>
# include <iostream>
# include "CgiEnvBuilder.hpp"

class CgiHandler {

	private:

		HttpRequest					*_req;
		ServerConfig				*_server;
		LocationConfig				*_location;
		std::string					_extension;
		std::string					_requestPath;
		std::string					_clientIp;

		std::string					_scriptPath;
		std::string					_interpreterPath;
		std::vector<std::string>	_envv;

	public:

		CgiHandler(HttpRequest *req, std::string clientIp, std::vector<ServerConfig *> servers);

		bool	isCgiRequest(void);
		void	resolveScriptPath(void);
		void	resolveInterpreterPath(void);
		void	buildEnv(void);
};

#endif