#ifndef CLIENTMAXBODYSIZESTRATEGY_HPP
# define CLIENTMAXBODYSIZESTRATEGY_HPP

# include "IValidationStrategy.hpp"

class ClientMaxBodySizeStrategy : public IValidationStrategy {

	public:
		virtual bool	validate(const std::string &value) const;
};

#endif