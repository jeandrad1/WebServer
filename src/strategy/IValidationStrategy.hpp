#ifndef IVALIDATIONSTRATEGY_HPP
# define IVALIDATIONSTRATEGY_HPP

# include <string>
# include <iostream>

class IValidationStrategy {

	public:
		virtual	~IValidationStrategy() {}
		virtual bool	validate(const std::string &value) const = 0;
};

#endif
