#include "../builder/HttpConfig.hpp"
#include "../builder/ServerConfig.hpp"

///////////////////////////////////////////////////
//             NO REFACTORIZAR                   //
///////////////////////////////////////////////////

class inheritance : public HttpConfig, public ServerConfig
{
	private:
		void takeValues(IConfig &builtConfig);
		void setValues(IConfig &builtConfig);
	public:
		inheritance();
		~inheritance();

        void runInheritance(std::vector<IConfig *> &builtConfigs);
};