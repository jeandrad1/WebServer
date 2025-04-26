#include "../builder/HttpConfig.hpp"
#include "../builder/ServerConfig.hpp"

#include "HttpToServerInheritance.hpp"
#include "ServerToLocationInheritance.hpp"

class InheritanceEngine {
	public:
		void inherit(HttpConfig& http);
	
		void inherit(ServerConfig& server);

		void runInherit(std::vector<IConfig *> &builtConfigs);
	};