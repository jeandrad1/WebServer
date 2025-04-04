#ifndef ReturnStrategy_HPP
# define ReturnStrategy_HPP

# include "IValidationStrategy.hpp"
# include <vector>

class ReturnStrategy : public IValidationStrategy
{

	public:
		bool	validate(const std::string &value) const;
};

#endif