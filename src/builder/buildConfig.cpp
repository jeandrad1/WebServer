# include <fstream>
# include <vector>
# include "../composite/ServerBlock.hpp"
# include "../composite/HttpBlock.hpp"
# include "../composite/LocationBlock.hpp"
# include "../factory/StrategyFactory.hpp"

#include "HttpConfig.hpp"
#include "ServerConfig.hpp"
#include "LocationBuilder.hpp"
#include "ServerBuilder.hpp"
#include "HttpBuilder.hpp"
#include "../composite/Directive.hpp"
#include "../utils/colors.hpp"

class LocationConfig;

std::vector<IConfig *> buildConfig(AConfigBlock *rawConfig)
{
    std::vector<IConfig *> builtConfigs;

    if (!rawConfig)
    {
        std::cerr << "Error: config_ptr is null." << std::endl;
        return builtConfigs;
    }
    for (AConfigBlock::iterator it = rawConfig->begin(); it != rawConfig->end(); ++it)
    {
		if (HttpBlock *httpBlock = dynamic_cast<HttpBlock *>(*it))
		{
            HttpBuilder httpBuilder;
            HttpConfig *http = static_cast<HttpConfig *>(httpBuilder.build(httpBlock));
            builtConfigs.push_back(http);
        }
        else if (ServerBlock *serverBlock = dynamic_cast<ServerBlock *>(*it))
        {
            ServerBuilder serverBuilder;
            ServerConfig *server = static_cast<ServerConfig *>(serverBuilder.build(serverBlock));
            builtConfigs.push_back(server);
        }
		else
		{
			std::cout << "Error: Unknown block type encountered." << std::endl;
		}
	}
	return builtConfigs;
}