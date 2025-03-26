#include "../../include/factory/StrategyFactory.hpp"
#include "../../include/factory/createStrategies.hpp"

IValidationStrategy *createErrorPageStrategy(void)
{
    return (new ErrorPageStrategy());
}

IValidationStrategy *createAutoIndexStrategy(void)
{
    return (new AutoindexStrategy());
}

IValidationStrategy *createClientMaxBodySizeStrategy(void)
{
    return (new ClientMaxBodySizeStrategy());
}

/*IValidationStrategy *createIndexStrategy(void)
{
    return (new IndexStrategy());
}

IValidationStrategy *createRootStrategy(void)
{
    return (new RootStrategy());
}

IValidationStrategy *createServerNameStrategy(void)
{
    return (new ServerNameStrategy());
}*/
