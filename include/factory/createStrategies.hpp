#ifndef CREATESTRATEGIES_HPP
# define CREATESTRATEGIES_HPP

# include "../strategy/strategies/AutoindexStrategy.hpp"
# include "../strategy/strategies/ClientMaxBodySizeStrategy.hpp"
# include "../strategy/strategies/ErrorPageStrategy.hpp"
# include "../strategy/strategies/IndexStrategy.hpp"
# include "../strategy/strategies/RootStragegy.hpp"
# include "../strategy/strategies/ServerNameStrategy.hpp"

IValidationStrategy *createAutoIndexStrategy(void);
IValidationStrategy *createClientMaxBodySizeStrategy(void);
IValidationStrategy *createErrorPageStrategy(void);
IValidationStrategy *createIndexStrategy(void);
IValidationStrategy *createRootStrategy(void);
IValidationStrategy *createServerNameStrategy(void);

#endif