#include "../builder/HttpConfig.hpp"
#include "../builder/ServerConfig.hpp"

class HttpToServerInheritance {
	public:
		void inherit(ServerConfig &server, HttpConfig &http);

		void configureClientMaxBodySize(ServerConfig &server, HttpConfig &http);
		void configureErrorPage(ServerConfig &server, HttpConfig const &http);
	};