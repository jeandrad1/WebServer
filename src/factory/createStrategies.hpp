#ifndef CREATESTRATEGIES_HPP
# define CREATESTRATEGIES_HPP

# include "../strategy/AutoindexStrategy.hpp"
# include "../strategy/ClientMaxBodySizeStrategy.hpp"
# include "../strategy/ErrorPageStrategy.hpp"
# include "../strategy/IndexStrategy.hpp"
# include "../strategy/ListenStrategy.hpp"
# include "../strategy/RootStrategy.hpp"
# include "../strategy/ServerNameStrategy.hpp"
# include "../strategy/ReturnStrategy.hpp"
# include "../strategy/LimitExceptStrategy.hpp"
# include "../strategy/CGIStrategy.hpp"

# include "../strategy/ServerBlockStrategy.hpp"
# include "../strategy/HttpBlockStrategy.hpp"
# include "../strategy/LocationBlockStrategy.hpp"

IValidationStrategyBlock	*createServerBlockStrategy(void);
IValidationStrategyBlock	*createHttpBlockStrategy(void);
IValidationStrategyBlock	*createLocationBlockStrategy(void);

IValidationStrategy			*createAutoIndexStrategy(void);
IValidationStrategy			*createClientMaxBodySizeStrategy(void);
IValidationStrategy			*createErrorPageStrategy(void);
IValidationStrategy			*createIndexStrategy(void);
IValidationStrategy			*createListenStrategy(void);
IValidationStrategy			*createRootStrategy(void);
IValidationStrategy			*createReturnStrategy(void);
IValidationStrategy			*createServerNameStrategy(void);
IValidationStrategy			*createLimitExceptStrategy(void);
IValidationStrategy			*createCGIStrategy(void);

#endif
