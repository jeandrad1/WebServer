#include "../../include/factory/StrategyFactory.hpp"
#include "../../include/factory/createStrategies.hpp"

void registerAllStrategies(StrategyFactory factory)
{
    StrategyFactory::getInstance().registerStrategy("autoindex", createAutoIndexStrategy().validate());
}