#include "../../include/factory/StrategyFactory.hpp"
#include "../../include/factory/createStrategies.hpp"

/**********************************************************************/
/*                      Create Block Strategies                       */
/**********************************************************************/

IValidationStrategyBlock *createServerBlockStrategy(void)
{
    return (new ServerBlockStrategy());
}

IValidationStrategyBlock *createHttpBlockStrategy(void)
{
    return (new HttpBlockStrategy());
}

IValidationStrategyBlock *createLocationBlockStrategy(void)
{
    return (new LocationBlockStrategy());
}

/**********************************************************************/
/*                   Create Directives Strategies                     */
/**********************************************************************/

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

IValidationStrategy *createIndexStrategy(void)
{
    return (new IndexStrategy());
}

IValidationStrategy *createListenStrategy(void)
{
    return (new ListenStrategy());
}

IValidationStrategy *createRootStrategy(void)
{
    return (new RootStrategy());
}

IValidationStrategy *createReturnStrategy(void)
{
    return (new ReturnStrategy());
}

IValidationStrategy *createServerNameStrategy(void)
{
    return (new ServerNameStrategy());
}
