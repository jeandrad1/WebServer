#include "inheritance.hpp"
#include "../utils/colors.hpp"

///////////////////////////////////////////////////
//             NO REFACTORIZAR                   //
///////////////////////////////////////////////////

void InheritanceEngine::inherit(HttpConfig &http)
{
	std::vector<ServerConfig*>::iterator it = http.servers.begin();
	for (; it != http.servers.end(); ++it)
	{
		inheritFromHttp(**it, http);
		inherit(**it);
	}
}

void InheritanceEngine::inherit(ServerConfig& server) 
{
	std::vector<LocationConfig*>::iterator it = server.locations.begin();
	for (; it != server.locations.end(); ++it) 
	{
		inheritFromServer(**it, server);
	}
}

void InheritanceEngine::inheritFromHttp(ServerConfig& server, const HttpConfig& http) 
{
	if (server.clientMaxBodySize == DEFAULT_CLIENT_MAX_BODY_SIZE && http.clientMaxBodySize != DEFAULT_CLIENT_MAX_BODY_SIZE)
	{
		server.clientMaxBodySize = http.clientMaxBodySize;
	}
	if (http.errorPageDirective == true && server.errorPageDirective == false)
	{
		server.errorPageDirective = http.errorPageDirective;
		server.errorPages = http.errorPages;
	}
}

void InheritanceEngine::inheritFromServer(LocationConfig& location, const ServerConfig& server) 
{
	if(location.clientMaxBodySize == DEFAULT_CLIENT_MAX_BODY_SIZE && location.clientMaxBodySize == DEFAULT_CLIENT_MAX_BODY_SIZE)
	{
		location.clientMaxBodySize = server.clientMaxBodySize;
	}
	if(server.errorPageDirective == true && location.errorPageDirective == false)
	{
		location.errorPageDirective = server.errorPageDirective;
		location.errorPages = server.errorPages;
	}
	if(location.root.empty())
	{
		location.root = server.root;
	}
	if(location.index.empty())
	{
		location.index = server.index;
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