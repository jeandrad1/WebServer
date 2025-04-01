#ifndef AUTOINDEXSTRATEGY_HPP
# define AUTOINDEXSTRATEGY_HPP

# include "IValidationStrategy.hpp"

class AutoindexStrategy : public IValidationStrategy {

	public:
		virtual bool validate(const std::string &value) const;
};

#endif