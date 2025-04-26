#include "../builder/ServerConfig.hpp"
#include "../builder/LocationConfig.hpp"

class ServerToLocationInheritance {
	public:
		void inherit(LocationConfig &location, ServerConfig &server);

		void configureClientMaxBodySize(LocationConfig &location, ServerConfig &server);
		void configureErrorPage(LocationConfig &location, const ServerConfig &server);
		void configureRoot(LocationConfig &location, ServerConfig &server);
		void configureIndex(LocationConfig &location, ServerConfig &server);

		void noLocations(ServerConfig &server);
	};