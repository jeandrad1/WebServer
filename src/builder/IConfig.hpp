#ifndef ICONFIG_HPP
#define ICONFIG_HPP

# include "../utils/colors.hpp"

class IConfig
{
	public:
		virtual			~IConfig() {}
		virtual void	printValues(int indent) = 0;
};

#endif
