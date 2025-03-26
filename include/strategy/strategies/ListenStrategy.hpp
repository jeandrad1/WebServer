#ifndef LISTENSTRATEGY_HPP
# define LISTENSTRATEGY_HPP

# include "../IValidationStrategy.hpp"

class ListenStrategy : public IValidationStrategy
{
	public:
		bool validate(const std::string &value) const;
};

#endif