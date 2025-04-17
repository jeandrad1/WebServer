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

std::vector<AConfigBlock *> BuildConfig(AConfigBlock *config_ptr)
{
    std::vector<AConfigBlock *> builtConfigs;

    if (!config_ptr) {
        std::cerr << "Error: config_ptr is null." << std::endl;
        return builtConfigs;
    }
    std::cout << "Building configuration..." << std::endl;
    for (AConfigBlock::iterator it = config_ptr->begin(); it != config_ptr->end(); ++it)
    {
		std::cout << CYAN"Processing block..." << WHITE << std::endl;

		HttpBlock *httpBlock = dynamic_cast<HttpBlock *>(*it);
		ServerBlock *serverBlock = dynamic_cast<ServerBlock *>(*it);
		LocationBlock *locationBlock = dynamic_cast<LocationBlock *>(*it);

		if (httpBlock)
		{
			std::cout << GREEN"ENTRA EN HTTP\n" << WHITE;
            HttpBuilder httpBuilder;
            std::cout << "Building HTTP block..." << std::endl;
            for (AConfigBlock::iterator blockIt = httpBlock->begin(); blockIt != httpBlock->end(); ++blockIt)
            {
                if (Directive *directive = dynamic_cast<Directive *>(*blockIt))
                    httpBuilder.dispatchDirective(directive->getName(), directive->getValue());
                else
                {
                    std::cout << "Building Server block..." << std::endl;
                    ServerBuilder *serverBuilder = new ServerBuilder();
                    std::cout << "new ServerBuilder done..." << std::endl;
                    httpBuilder.addNestedBuilder(serverBuilder);
                    std::cout << "addNestedBuilder done..." << std::endl;
                    delete serverBuilder;
                }
            }
            builtConfigs.push_back(static_cast<AConfigBlock *>(httpBuilder.build()));
        }
        else if (serverBlock)
        {
			std::cout << RED"Entra en server\n" << WHITE;
            ServerBuilder serverBuilder;
            std::cout << "Building Server block..." << std::endl;
            for (AConfigBlock::iterator blockIt = serverBlock->begin(); blockIt != serverBlock->end(); ++blockIt)
            {
                std::cout << "Processing Server block..." << std::endl;
                if (Directive *directive = dynamic_cast<Directive *>(*blockIt))
                    serverBuilder.dispatchDirective(directive->getName(), directive->getValue());
                else
                {
                    LocationBuilder *locationBuilder = new LocationBuilder((*blockIt)->getName());
                    serverBuilder.addNestedBuilder(locationBuilder);
                    delete locationBuilder;
                }
            }
            builtConfigs.push_back(static_cast<AConfigBlock *>(serverBuilder.build()));
        }
        else if (locationBlock)
		{
			LocationBuilder locationBuilder(locationBlock->getName());
			AConfigBlock::iterator blockItEnd = locationBlock->end();
			for (AConfigBlock::iterator blockIt = locationBlock->begin(); blockIt != blockItEnd; blockIt++)
			{
				if (Directive *directive = dynamic_cast<Directive *>(*blockIt))
					locationBuilder.dispatchDirective(directive->getName(), directive->getValue());
			}
			builtConfigs.push_back(static_cast<AConfigBlock *>(locationBuilder.build()));
		}
		else
		{
			std::cout << "Error: Unknown block type encountered." << std::endl;
		}
	}
	return builtConfigs;
}