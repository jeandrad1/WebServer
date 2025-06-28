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

	private:
		std::string						_locationPath;
		std::string						_root;
		std::vector<std::string>		_index;
		bool							_autoindex;
		long long						_clientMaxBodySize;
		t_return						*_return;
		std::map<int, t_errorPage *>	_errorPages;
		bool							_errorPageDirective;
		t_limit_except					*_limit_except;

	public:
		virtual							~LocationConfig();
		virtual void					printValues(int indent = 0);

		void							applyInheritedConfig(void);
		void							defaultInheritedValues(void);
		void							inheritFromServer(const ServerConfig &server);

		void							setLocationPath(std::string &src);
		void							setRoot(std::string &src);
		void							setIndex(std::vector<std::string> &src);
		void							setAutoIndex(bool src);
		void							setClientMaxBodySize(long long src);
		void							setErrorPages(std::map<int, t_errorPage *> &src);
		void							setErrorPageDirective(bool src);

		void							setReturn(t_return *src);
		void							setReturnHttp(std::string &http);
		void							setReturnCode(int src);
		void							setReturnDirective(bool src);

		void							setLimitExcept(t_limit_except *src);
		void							setLimitExceptLimitDirective(bool src);
		void							setLimitExceptPost(bool src);
		void							setLimitExceptGet(bool src);
		void							setLimitExceptDelete(bool src);


		std::string								getLocationPath() const;
		std::string								getRoot() const;
		const std::vector<std::string>			getIndex()const;
		bool									getAutoIndex() const;
		long long								getClientMaxBodySize() const;
		t_return								*getReturn() const;
		const std::map<int, t_errorPage *>		getErrorPages() const;
		bool									getErrorPageDirective() const;
		t_limit_except							*getLimitExcept() const;
};

#endif
