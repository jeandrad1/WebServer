#include "../../include/factory/StrategyFactory.hpp"

void registerAllStrategies(StrategyFactory factory);

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

/***********************************************************************/
/*                         Operator Overload                           */
/***********************************************************************/


/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

StrategyFactory &StrategyFactory::getInstance(void)
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
    if (this->strategyMap.find(name) != strategyMap.end())
    {
        return (strategyMap[name]());
    }
    else
    {
        std::cout << "No strategy detected, name: " << name << "\n";
        return (NULL);
    }
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/