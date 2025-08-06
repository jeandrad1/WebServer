#ifndef SERVERUTILS_HPP
# define SERVERUTILS_HPP

# include "../builder/ServerConfig.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include <netinet/in.h>
# include <fcntl.h>

class ServerUtils {

	public:

		static ServerConfig			*getServerByHostName(std::string host, std::vector<ServerConfig *> servers);
		static LocationConfig		*getLocationByRequestPath(std::string path, ServerConfig *server);

		static std::string			getClientIP(struct sockaddr_in &client_addr);
		static std::string			resolveScriptPath(const HttpRequest *req, const LocationConfig *location);
		static std::string			resolveInterpreterPath(LocationConfig *location, std::string extension);
		static std::vector<char>	prepareResponseBuffer(const HttpResponse& response);
		static void					sendErrorResponse(int clientFd, std::vector<char> responseBuffer);
		static void					setNotBlockingFd(int fd);
};

#endif