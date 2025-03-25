#ifndef INDEXSTRATEGY_HPP
# define INDEXSTRATEGY_HPP

# include "IValidationStrategy.hpp"

class IndexStrategy : public IValidationStrategy
{

	public:
		bool validate(const std::string &value) const;
};

#endif