# include <fstream>
# include <vector>
# include "../composite/ServerBlock.hpp"
# include "../composite/HttpBlock.hpp"
# include "../composite/LocationBlock.hpp"
# include "../factory/StrategyFactory.hpp"

#include "HttpConfig.hpp"
#include "ServerConfig.hpp"
#include "LocationConfig.hpp"
#include "LocationBuilder.hpp"
#include "ServerBuilder.hpp"
#include "HttpBuilder.hpp"
#include "Directive.hpp"

std::vector<AConfigBlock *> BuildConfig(AConfigBlock *config_ptr)
{
    std::vector<AConfigBlock *> builtConfigs;

    for (AConfigBlock::iterator it = config_ptr->begin(); it != config_ptr->end(); ++it)
    {
        if (HttpBlock *httpBlock = dynamic_cast<HttpBlock *>(*it))
        {
            HttpBuilder httpBuilder;
            for (AConfigBlock::iterator blockIt = httpBlock->begin(); blockIt != httpBlock->end(); ++blockIt)
            {
                if (Directive *directive = dynamic_cast<Directive *>(*blockIt))
                    httpBuilder.dispatchDirective(directive->getName(), directive->getValue());
                else
                {
                    ServerBuilder *serverBuilder = new ServerBuilder();
                    httpBuilder.addNestedBuilder(serverBuilder);
                }
            }
            builtConfigs.push_back(static_cast<AConfigBlock *>(httpBuilder.build()));
        }

        else if (ServerBlock *serverBlock = dynamic_cast<ServerBlock *>(*it))
        {
            ServerBuilder serverBuilder;
            for (AConfigBlock::iterator blockIt = serverBlock->begin(); blockIt != serverBlock->end(); ++blockIt)
            {
                if (Directive *directive = dynamic_cast<Directive *>(*blockIt))
                    serverBuilder.dispatchDirective(directive->getName(), directive->getValue());
                else
                {
                    LocationBuilder *locationBuilder = new LocationBuilder();
                    serverBuilder.addNestedBuilder(locationBuilder);
                }
            }
            builtConfigs.push_back(static_cast<AConfigBlock *>(serverBuilder.build()));
        }

        else if (LocationBlock *locationBlock = dynamic_cast<LocationBlock *>(*it))
        {
            LocationBuilder locationBuilder;
            for (AConfigBlock::iterator blockIt = locationBlock->begin(); blockIt != locationBlock->end(); ++blockIt)
            {
                if (Directive *directive = dynamic_cast<Directive *>(*blockIt))
                    locationBuilder.dispatchDirective(directive->getName(), directive->getValue());
            }
            builtConfigs.push_back(static_cast<AConfigBlock *>(locationBuilder.build()));
        }
    }
    return builtConfigs;
}