#include "StrategyFactory.hpp"
#include "createStrategies.hpp"

void	registerAllStrategies(StrategyFactory factory)
{
	StrategyFactory::getInstance().registerStrategy("autoindex", createAutoIndexStrategy);
	StrategyFactory::getInstance().registerStrategy("client_max_body_size", createClientMaxBodySizeStrategy);
	StrategyFactory::getInstance().registerStrategy("error_page", createErrorPageStrategy);
	StrategyFactory::getInstance().registerStrategy("index", createIndexStrategy);
	StrategyFactory::getInstance().registerStrategy("listen", createListenStrategy);
	StrategyFactory::getInstance().registerStrategy("return", createReturnStrategy);
	StrategyFactory::getInstance().registerStrategy("root", createRootStrategy);
	StrategyFactory::getInstance().registerStrategy("server_name", createServerNameStrategy);
}

void	registerBlockStrategies(StrategyFactory factory)
{
	StrategyFactory::getInstance().registerStrategy("server", createServerBlockStrategy);
	StrategyFactory::getInstance().registerStrategy("http", createHttpBlockStrategy);
	StrategyFactory::getInstance().registerStrategy("location", createLocationBlockStrategy);
}
