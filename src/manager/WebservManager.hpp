#ifndef WEBSERVMANAGER_HPP
# define WEBSERVMANAGER_HPP

# include <string>
# include <map>
# include <vector>
#include <fstream>
# include "../composite/AConfigBlock.hpp"
# include "../builder/ServerConfig.hpp"
# include "../execute/EventLoop.hpp"

class WebservManager {

	public:
		WebservManager(const std::string &configPath);
		~WebservManager();

		void	run(void);

	private:
		std::ifstream								_configFile;
		AConfigBlock								*_rootBlock;
		std::vector<IConfig *>						builtConfigs;

        std::map<int, std::vector<ServerConfig *> >	servers;
		void serversMapConstructor(std::vector<IConfig *> &builtConfigs);
		void serversMapConstructor(std::vector<ServerConfig *> &builtConfigs);
		void impressMapServer(std::map<int, std::vector<ServerConfig *> > &serversMap);
};

#endif
