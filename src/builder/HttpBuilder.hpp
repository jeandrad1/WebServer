#ifndef HTTPBUILDER_HPP
# define HTTPBUILDER_HPP

# include "IConfigBuilder.hpp"
# include "DirectiveProcessor.hpp"
# include "HttpConfig.hpp"

class HttpBuilder : public IConfigBuilder, public DirectiveProcessor<HttpBuilder> {

	private:
		HttpConfig	*http;
		bool		built;
		t_cgi		*cgi;

	public:
		HttpBuilder();
		~HttpBuilder();

		void	setDirective(const std::string &name, const std::string &value);
		void	addNestedBuilder(IConfigBuilder *child, AConfigBlock *newConfig);
		IConfig	*build(AConfigBlock *httpBlock);
		void	setDefaultValues(void);

		void	handleErrorPage(const std::string &value);
		void	handleClientMaxBodySize(const std::string &value);
		void	handleCGI(const std::string &value);
};

#endif
