#ifndef ICONFIG_HPP
#define ICONFIG_HPP

# include "../utils/colors.hpp"
# include "structsBuilder.hpp"

class IConfig
{
	public:
		virtual			~IConfig() {}
		virtual void	printValues(int indent) = 0;

		/////////////
		//inherance//
		/////////////

		virtual void	inherance(void) = 0;
		virtual void	defaultInheritValues(void) = 0;
};

#endif
