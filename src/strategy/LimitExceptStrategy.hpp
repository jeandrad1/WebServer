#ifndef LIMITEXCEPT_HPP
# define LIMITEXCEPT_HPP

# include "IValidationStrategy.hpp"

class LimitExceptStrategy : public IValidationStrategy {

	public:
		bool	validate(const std::string &value) const;
};

#endif
