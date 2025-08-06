#ifndef ICONFIG_HPP
#define ICONFIG_HPP

# include "../utils/colors.hpp"
# include "structsBuilder.hpp"

class IConfig
{
	public:
		virtual			~IConfig() {}
		virtual void	printValues(int indent) = 0;

		virtual void	applyInheritedConfig(void) = 0;
		virtual void	defaultInheritedValues(void) = 0;
};

#endif
