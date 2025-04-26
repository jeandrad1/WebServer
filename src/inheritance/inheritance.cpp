#include "inheritance.hpp"
#include "../utils/colors.hpp"

///////////////////////////////////////////////////
//             NO REFACTORIZAR                   //
///////////////////////////////////////////////////

void InheritanceEngine::inherit(HttpConfig &http)
{
	HttpToServerInheritance httpToServer;
	std::vector<ServerConfig*>::iterator it = http.servers.begin();
	for (; it != http.servers.end(); ++it)
	{
		ServerConfig *server = *it;
		httpToServer.inherit(*server, http);
		inherit(*server);
	}
}

void InheritanceEngine::inherit(ServerConfig& server) 
{
	ServerToLocationInheritance serverToLocation;
	std::vector<LocationConfig*>::iterator it = server.locations.begin();
	for (; it != server.locations.end(); ++it) 
	{
		LocationConfig *location = *it;
		serverToLocation.inherit(*location, server);
	}
}

void InheritanceEngine::runInherit(std::vector<IConfig *> &builtConfigs)
{
	std::vector<IConfig*>::iterator it = builtConfigs.begin();
	for (; it != builtConfigs.end(); ++it)
	{
		HttpConfig* http = dynamic_cast<HttpConfig*>(*it);
		if (http)
		{
			inherit(*http);
			continue;
		}
		ServerConfig* server = dynamic_cast<ServerConfig*>(*it);
		if (server)
		{
			inherit(*server);
			continue;
		}
	}
}