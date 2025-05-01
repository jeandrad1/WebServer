#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include <string>
# include <vector>
# include <cstdlib>
# include <sstream>
# include <iostream>
# include <map>
# include "IConfig.hpp"

class ServerConfig;

class LocationConfig : public IConfig
{

	public:
		std::string						locationPath;
		std::string						root;
		std::vector<std::string>		index;
		bool							autoindex;
		long long						clientMaxBodySize;
		t_return						*_return;
		std::map<int, t_errorPage *>	errorPages;
		bool							errorPageDirective;
		t_limit_except					*limit_except;

		virtual			~LocationConfig();
		virtual void	printValues(int indent = 0);

		/////////////
		//inherance//
		/////////////

		virtual void	inherance(void);
		virtual void	defaultInheritValues(void);
		void			inheritFromServer(const ServerConfig &server);
};

#endif
