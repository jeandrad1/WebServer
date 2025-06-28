#ifndef SERVERBUILDER_HPP
# define SERVERBUILDER_HPP

# include "IConfigBuilder.hpp"
# include "DirectiveProcessor.hpp"
# include "ServerConfig.hpp"
# include "LocationBuilder.hpp"

class ServerBuilder : public IConfigBuilder, public DirectiveProcessor<ServerBuilder>
{
	private:
		bool            built;
		ServerConfig    *server;

	public:
		ServerBuilder();
		~ServerBuilder();

		void	setDirective(const std::string &key,const std::string &value);
		void	addNestedBuilder(IConfigBuilder *child, AConfigBlock *newBlock);
		IConfig	*build(AConfigBlock *serverBlock); // checks the server values and if they are not set it sets them with default values
		void	setDefaultValues();

		void	handleListen(const std::string &value); 
		void	handleServerName(const std::string &value);
		void	handleRoot(const std::string &value);
		void	handleIndex(const std::string &value);
		void	handleClientMaxBodySize(const std::string &value);
		void	handleAutoindex(const std::string &value);
		void	handleErrorPage(const std::string &value);
		void	handleReturn(const std::string &value);
		void	handleCGI(const std::string &value);
};

#endif
