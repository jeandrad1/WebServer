#ifndef SERVERUTILS_HPP
# define SERVERUTILS_HPP

# include "../builder/ServerConfig.hpp"
# include <netinet/in.h>

class ServerUtils {

	public:

		static ServerConfig *getServerByHostName(std::string host, std::vector<ServerConfig *> servers);
		static LocationConfig *getLocationByRequestPath(std::string path, ServerConfig *server);

		static std::string getClientIP(struct sockaddr_in &client_addr);
};

#endif