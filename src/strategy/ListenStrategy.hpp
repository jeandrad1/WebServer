#ifndef LISTENSTRATEGY_HPP
# define LISTENSTRATEGY_HPP

# include "IValidationStrategy.hpp"
# include <cstdlib>
# include <sstream>

class ListenStrategy : public IValidationStrategy
{
	public:
		bool	validate(const std::string &value) const;
};

#endif