#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "IConfigBuilder.hpp"
# include "DirectiveProcessor.hpp"

# include "IConfig.hpp"
# include "LocationConfig.hpp"

class HttpConfig;

class ServerConfig : public IConfig
{

	public:
		std::vector<LocationConfig *>	locations;

		std::vector<t_listen *>			listen;
		bool							listenDirective;
		t_return						*_return;
		long long						clientMaxBodySize;
		bool							autoindex;
		std::string						root;
		bool							errorPageDirective;
		std::map<int, t_errorPage *>	errorPages;
		std::vector<std::string>		serverNames;
		std::vector<std::string>		index;

		virtual			~ServerConfig();
		virtual void	printValues(int indent = 0);

		/////////////
		//inherance//
		/////////////

		virtual void	inherance(void);
		void			inheritFromHttp(const HttpConfig &http);
		void			defaultInheritValues(void);
};

#endif