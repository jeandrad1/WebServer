#include "../builder/HttpConfig.hpp"
#include "../builder/ServerConfig.hpp"

///////////////////////////////////////////////////
//             NO REFACTORIZAR                   //
///////////////////////////////////////////////////

class InheritanceEngine {
	public:
		void inherit(HttpConfig& http);
	
		void inherit(ServerConfig& server);
	
		void inheritFromHttp(ServerConfig& server, const HttpConfig& http);

		void inheritFromServer(LocationConfig& location, const ServerConfig& server);

		void runInherit(std::vector<IConfig *> &builtConfigs);
	};