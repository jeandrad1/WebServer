#include "WebservManager.hpp"


#include <fstream>
#include "../composite/ServerBlock.hpp"
#include "../factory/StrategyFactory.hpp"
#include "../execute/SocketsManager.hpp"
void					printBuiltConfigs(const std::vector<IConfig *> &builtConfigs);
std::vector<IConfig *>	createConfigClasses(AConfigBlock *config_ptr);


AConfigBlock			*createBlock(std::ifstream &filename, AConfigBlock &block);
int						validateConfigTreeFactory(AConfigBlock &config);
void					registerAllStrategies(void);
void					registerBlockStrategies(void);

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

WebservManager::WebservManager(const std::string &configPath) : _configFile(configPath.c_str())
{
	if (!_configFile.is_open())
	{
		std::cerr << "Error: Could not open the file.\n";
	}

	registerAllStrategies();
	registerBlockStrategies();
}

WebservManager::~WebservManager(void)
{
	this->_configFile.close();
	
	std::vector<IConfig * >::const_iterator ite = this->builtConfigs.end();
	for (std::vector<IConfig *>::const_iterator it = this->builtConfigs.begin(); it != ite; ++it)
	{
		delete (*it);
	}
}

/***********************************************************************/
/*                         Operator Overload                           */
/***********************************************************************/

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

void WebservManager::run(void)
{
	ServerBlock		config("Config");

	this->_rootBlock = createBlock(this->_configFile, config);

	//this->_rootBlock->getBlock(0)->printConfig(0);

	validateConfigTreeFactory((*this->_rootBlock));

	this->builtConfigs = createConfigClasses(this->_rootBlock);

	//printBuiltConfigs(this->builtConfigs);

	serversMapConstructor(builtConfigs);

	impressMapServer(servers);

	SocketsManager sockets;
	EpollManager epoll;
	sockets.createSockets(servers);
	EventLoop loop(epoll, sockets.getServerSockets(), this->servers);

	loop.runEventLoop();        // listen, epoll and serve
	std::cout << "runEpollLopp done" << std::endl;
	sockets.printServerSockets();
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/


/***********************************************************************/
/*                          Private Functions                          */
/***********************************************************************/

#include "../builder/HttpConfig.hpp"
#include "../builder/ServerConfig.hpp"

void WebservManager::serversMapConstructor(std::vector<ServerConfig *> &builtConfigs)
{
	for(std::vector<ServerConfig *>::iterator its = builtConfigs.begin(); its != builtConfigs.end(); its++)
	{
		for(std::vector<t_listen *>::iterator itl = (*its)->listen.begin(); itl != (*its)->listen.end(); itl++)
		{
			std::vector<ServerConfig *> &vec = this->servers[(*itl)->port];

			bool alreadyExists = false;

			for (std::vector<ServerConfig *>::iterator it = vec.begin(); it != vec.end(); it++)
			{
				if (*it == *its)
				{
					alreadyExists = true;
					break;
				}
			}
			if(alreadyExists == false)
				vec.push_back(*its);
		}
	}
}

void WebservManager::serversMapConstructor(std::vector<IConfig *> &builtConfigs)
{
	for(std::vector<IConfig *>::iterator iti = builtConfigs.begin(); iti != builtConfigs.end(); iti++)
	{
		if (HttpConfig *http = dynamic_cast<HttpConfig *>(*iti))
			serversMapConstructor(http->servers);
		if (ServerConfig *server = dynamic_cast<ServerConfig *>(*iti))
		{
			for(std::vector<t_listen *>::iterator itl = server->listen.begin(); itl != server->listen.end(); itl++)
			{
				std::vector<ServerConfig *> &vec = this->servers[(*itl)->port];

				bool alreadyExists = false;

				for (std::vector<ServerConfig *>::iterator it = vec.begin(); it != vec.end(); it++)
				{
					if (*it == server)
					{
						alreadyExists = true;
						break;
					}
				}
				if(alreadyExists == false)
					vec.push_back(server);
			}
		}
	}
}

void WebservManager::impressMapServer(std::map<int, std::vector<ServerConfig *> > &serversMap)
{
	for (std::map<int, std::vector<ServerConfig *> >::iterator it = serversMap.begin(); it != serversMap.end(); it++)
	{
		std::cout << YELLOW << (*it).first << WHITE << ":\n";
		for(std::vector<ServerConfig *>::iterator its = (*it).second.begin(); its != (*it).second.end(); its++)
		{
			std::cout << GREEN << " CLientMaxBodySize_Server -> [" << BLUE << (*its)->clientMaxBodySize << GREEN "]\n" RESET;
		}
	}
}