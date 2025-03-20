#ifndef IVALIDATIONSTRATEGY_HPP
# define IVALIDATIONSTRATEGY_HPP

# include <string>

class IValidationStrategy {

	public:
		virtual ~IValidationStrategy() {}
		virtual bool validate(const std::string &value) const = 0;
};

#endif