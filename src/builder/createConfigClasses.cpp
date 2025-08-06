#include "../composite/ServerBlock.hpp"
#include "../composite/HttpBlock.hpp"
#include "../composite/LocationBlock.hpp"
#include "../composite/Directive.hpp"

#include "HttpBuilder.hpp"
#include "ServerBuilder.hpp"
#include "LocationBuilder.hpp"

#include "../utils/colors.hpp"

std::vector<IConfig *>	createConfigClasses(AConfigBlock *rawConfig)
{
	std::vector<IConfig *>	builtConfigs;
	if (!rawConfig)
	{
		std::cerr << RED "Error:" RESET << " config_ptr is null.\n";
		return (builtConfigs);
	}
	AConfigBlock::iterator	ite = rawConfig->end();
	for (AConfigBlock::iterator	it = rawConfig->begin(); it != ite; it++)
	{
		if (HttpBlock	*httpBlock = dynamic_cast<HttpBlock *>(*it))
		{
			HttpBuilder	httpBuilder;
			HttpConfig	*http = static_cast<HttpConfig *>(httpBuilder.build(httpBlock));
			builtConfigs.push_back(http);
		}
		else if (ServerBlock	*serverBlock = dynamic_cast<ServerBlock *>(*it))
		{
			ServerBuilder	serverBuilder;
			ServerConfig	*server = static_cast<ServerConfig *>(serverBuilder.build(serverBlock));
			builtConfigs.push_back(server);
		}
		else
		{
			std::cout << RED "Error:" RESET << " Unknown block type encountered.\n";
		}
	}
	for (size_t i = 0; i < builtConfigs.size(); ++i)
		builtConfigs[i]->applyInheritedConfig();
	return (builtConfigs);
}
