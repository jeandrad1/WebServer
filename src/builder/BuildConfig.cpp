#include <fstream>
#include <vector>
#include "../composite/ServerBlock.hpp"
#include "../composite/HttpBlock.hpp"
#include "../factory/StrategyFactory.hpp"

#include "HttpConfig.hpp"

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
                    httpBuilder.addNestedBuilder(new ServerBuilder());
            }
            builtConfigs.push_back(static_cast<HttpConfig *>(httpBuilder.build()));
        }

        else if (ServerBlock *serverBlock = dynamic_cast<ServerBlock *>(*it))
        {
            ServerBuilder serverBuilder;
            for (AConfigBlock::iterator blockIt = serverBlock->begin(); blockIt != serverBlock->end(); ++blockIt)
            {
                if (Directive *directive = dynamic_cast<Directive *>(*blockIt))
                    serverBuilder.dispatchDirective(directive->getName(), directive->getValue());
                else
                    serverBuilder.addNestedBuilder(new LocationBuilder());
            }
            builtConfigs.push_back(static_cast<ServerConfig *>(serverBuilder.build()));
        }

        else if (LocationBlock *httpBlock = dynamic_cast<LocationBlock *>(*it))
        {
            LocationBuilder locationBuilder;
            for (AConfigBlock::iterator blockIt = locationBlock->begin(); blockIt != locationBlock->end(); ++blockIt)
            {
                if (Directive *directive = dynamic_cast<Directive *>(*blockIt))
                    locationBuilder.dispatchDirective(directive->getName(), directive->getValue());
            }
            builtConfigs.push_back(static_cast<LocationConfig *>(locationBuilder.build()));
        }
    }
    return builtConfigs;
}
