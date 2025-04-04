#ifndef SERVERNAMESTRATEGY_HPP
#define SERVERNAMESTRATEGY_HPP

# include "IValidationStrategy.hpp"
# include <cctype>

class ServerNameStrategy : public IValidationStrategy
{

	public:
		bool	validate(const std::string &value) const;
};

#endif
