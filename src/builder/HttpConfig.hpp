#ifndef HTTPCONFIG_HPP
# define HTTPCONFIG_HPP

# include "LocationConfig.hpp"
# include "ServerBuilder.hpp"
# include "IConfig.hpp"

class HttpConfig : public IConfig {

	public:
		std::vector<ServerConfig *>		servers;

		long							clientMaxBodySize;
		std::map<int, t_errorPage *>	errorPages;
		bool							errorPageDirective;

		virtual			~HttpConfig();
		virtual void	printValues(int indent = 0);

		void	applyInheritedConfig(void);
		void	defaultInheritedValues(void);
};

#endif
