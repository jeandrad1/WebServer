#ifndef ERRORPAGESTRATEGY_HPP
# define ERRORPAGESTRATEGY_HPP

# include "../IValidationStrategy.hpp"

class ErrorPageStrategy : public IValidationStrategy {

	public:
		virtual bool validate(const std::string &value) const;
};

#endif