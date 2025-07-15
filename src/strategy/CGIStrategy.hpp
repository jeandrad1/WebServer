#ifndef CGISTRATEGY_HPP
# define CGISTRATEGY_HPP

#include "IValidationStrategy.hpp"
#include <vector>

class CGIStrategy : public IValidationStrategy {

	private:
		std::vector<std::string> validExtensions;

		
		public:
		
		CGIStrategy(void);
		virtual bool	validate(const std::string &value) const;
};

#endif