#ifndef CGIENVBUILDER_HPP
# define CGIENVBUILDER_HPP

# include <string>
# include <vector>
# include <cstring>
# include "HttpRequest.hpp"
# include "../builder/ServerConfig.hpp"

class CgiEnvBuilder {

	private:

		std::vector<std::string> _envv;
		char					**_envv_array;
		const HttpRequest		*_req;
		const ServerConfig		*_server;
		const LocationConfig	*_location;
		const std::string		_clientIP;
	
	public:

		CgiEnvBuilder(HttpRequest *req, ServerConfig *server, LocationConfig *location, std::string clientIP);
		char	**build(void);
		void	addEnv(const std::string &key, const std::string &value);
		std::vector<std::string> getEnvv(void);
		char	**vectorToCharArray(void);

		void	printEnv(void);
};

#endif