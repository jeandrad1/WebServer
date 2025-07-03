#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "IConfigBuilder.hpp"
# include "DirectiveProcessor.hpp"

# include "IConfig.hpp"
# include "LocationConfig.hpp"

class HttpConfig;

class ServerConfig : public IConfig
{

	private:
		std::vector<LocationConfig *>	_locations;

		std::vector<t_listen *>			_listen;
		bool							_listenDirective;
		t_return						*_return;
		long long						_clientMaxBodySize;
		bool							_autoindex;
		std::string						_root;
		bool							_errorPageDirective;
		std::map<int, t_errorPage *>	_errorPages;
		std::vector<std::string>		_serverNames;
		std::vector<std::string>		_index;

	public:
		virtual			~ServerConfig();
		virtual void	printValues(int indent = 0);

		void			applyInheritedConfig(void);
		void			defaultInheritedValues(void);
		void			inheritFromHttp(const HttpConfig &http);

		void			setLocations(std::vector<LocationConfig*>& src);
		void			setListen(std::vector<t_listen *> &src);
		void			setListenDirective(bool src);

		void			setReturn(t_return *src);
		void			setReturnHttp(std::string &http);
		void			setReturnCode(int src);
		void			setReturnDirective(bool src);

		void			setClientMaxBodySize(long long src);
		void			setAutoIndex(bool src);
		void			setRoot(std::string &src);
		void			setErrorPages(std::map<int, t_errorPage *> &src);
		void			setErrorPageDirective(bool src);
		void			setServerNames(std::vector<std::string> &src);
		void			setIndex(std::vector<std::string> &src);

		void			addLocation(LocationConfig *location);
		void			addListen(t_listen *listen);
		void			addErrorPage(int src1, t_errorPage *src2);
		void			addServerNames(std::string serverName);
		void			addIndex(std::string Index);

		void			clearListen();
		void			clearServerName();

		const std::vector<LocationConfig*>		&getLocations() const;
		const std::vector<t_listen *>			&getListen() const;
		bool									getListenDirective() const;
		t_return								*getReturn() const;
		long long 								getClientMaxBodySize() const;
		bool									getAutoIndex() const;
		std::string								getRoot() const;
		const std::map<int, t_errorPage *>		getErrorPages() const;
		bool									getErrorPageDirective() const;
		std::vector<std::string>				getServerNames() const;
		std::vector<std::string>				getIndex() const;
};

#endif