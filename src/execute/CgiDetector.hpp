#ifndef CGIDETECTOR_HPP
# define CGIDETECTOR_HPP

# include <map>
# include "HttpRequest.hpp"
# include <vector>
# include "../builder/ServerConfig.hpp"

class CgiDetector {

	private:

		ServerConfig *getServerByHostName(std::string host, std::vector<ServerConfig *> servers);
		LocationConfig *getLocationByRequestPath(std::string path, ServerConfig *server);

	public:

		bool isCgiRequest(HttpRequest *req, std::vector<ServerConfig *> servers);
};

#endif