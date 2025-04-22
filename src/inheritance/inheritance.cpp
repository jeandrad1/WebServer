#include "inheritance.hpp"
#include "../utils/colors.hpp"

///////////////////////////////////////////////////
//             NO REFACTORIZAR                   //
///////////////////////////////////////////////////

inheritance::inheritance()
{}

inheritance::~inheritance()
{}

void inheritance::takeValues(IConfig &builtConfig)
{
	if (HttpConfig* http = dynamic_cast<HttpConfig*>(&builtConfig))
	{
		if (http->clientMaxBodySize != DEFAULT_CLIENT_MAX_BODY_SIZE)
		{
			this->HttpConfig::clientMaxBodySize = http->clientMaxBodySize;
		}
		if (http->errorPageDirective == true)
		{
			this->HttpConfig::errorPageDirective = http->errorPageDirective;
			this->HttpConfig::errorPages = http->errorPages;
		}
	}

    if (ServerConfig* server = dynamic_cast<ServerConfig*>(&builtConfig))
    {
		if(server->clientMaxBodySize != DEFAULT_CLIENT_MAX_BODY_SIZE)
		{
			this->ServerConfig::clientMaxBodySize = server->clientMaxBodySize;
		}
		if(server->errorPageDirective == true)
		{
			this->ServerConfig::errorPageDirective = server->listenDirective;
			this->ServerConfig::errorPages = server->errorPages;
		}
		if(!server->root.empty())
		{
			this->ServerConfig::root = server->root;
		}
		if(!server->index.empty())
		{
			this->ServerConfig::index = server->index;
		}
    }
}

void inheritance::setValues(IConfig &builtConfig)
{
	if (ServerConfig* server = dynamic_cast<ServerConfig*>(&builtConfig))
	{
		if (server->clientMaxBodySize == DEFAULT_CLIENT_MAX_BODY_SIZE)
		{
			server->clientMaxBodySize = this->HttpConfig::clientMaxBodySize;
		}
		if (server->errorPageDirective == false)
		{
			server->errorPageDirective = this->HttpConfig::errorPageDirective;
			server->errorPages = this->HttpConfig::errorPages;
		}
	}
	if (LocationConfig* location = dynamic_cast<LocationConfig*>(&builtConfig))
	{
		if(location->clientMaxBodySize == DEFAULT_CLIENT_MAX_BODY_SIZE)
		{
			location->clientMaxBodySize = ServerConfig::clientMaxBodySize;
		}
		if(location->errorPageDirective == true)
		{
			location->errorPageDirective = this->ServerConfig::errorPageDirective;
			location->errorPages = this->ServerConfig::errorPages;
		}
		if(location->root.empty())
		{
			location->root = this->ServerConfig::root;
		}
		if(location->index.empty())
		{
			location->index = this->ServerConfig::index;
		}
	}
}

void inheritance::runInheritance(std::vector<IConfig *> &builtConfigs)
{
	std::vector<IConfig *>::iterator ite = builtConfigs.end();
	for(std::vector<IConfig *>::iterator it = builtConfigs.begin(); it != ite; it++)
	{
		if (HttpConfig *http = dynamic_cast<HttpConfig *>(*it))
		{
			takeValues(*http);
			std::vector<IConfig*> convertedServers;
			for (std::vector<ServerConfig*>::iterator it = http->servers.begin(); it != http->servers.end(); ++it)
				convertedServers.push_back(static_cast<IConfig*>(*it));
			this->runInheritance(convertedServers);
		}
		if (ServerConfig *server = dynamic_cast<ServerConfig *>(*it))
		{
			takeValues(*server);
			setValues(*server);
			std::vector<IConfig*> convertedLocations;
			for (std::vector<LocationConfig*>::iterator it = server->locations.begin(); it != server->locations.end(); ++it)
				convertedLocations.push_back(static_cast<IConfig*>(*it));
			this->runInheritance(convertedLocations);
		}
		if (LocationConfig *location = dynamic_cast<LocationConfig *>(*it))
		{
			setValues(*location);
		}
	}
}