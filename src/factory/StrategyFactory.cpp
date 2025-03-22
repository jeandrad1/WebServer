#include "../../include/factory/StrategyFactory.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

/***********************************************************************/
/*                         Operator Overload                           */
/***********************************************************************/


/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

static StrategyFactory &StrategyFactory::getInstance(void)
{
    static StrategyFactory instance;
    return (instance); 
}

void StrategyFactory::registerStrategy(const std::string &name, StrategyCreateFuncPtr func)
{
    this->strategyMap[name] = func;
}

IValidationStrategy	*StrategyFactory::chooseStrategy(const std::string &name)
{
    if (strategyMap.find(name) != strategyMap.end())
    {
        return (strategyMap[name]());
    }
    else
        return (NULL);
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/