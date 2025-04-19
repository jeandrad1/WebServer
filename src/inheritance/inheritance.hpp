#include "../builder/HttpConfig.hpp"
#include "../builder/LocationConfig.hpp"
#include "../builder/ServerConfig.hpp"

///////////////////////////////////////////////////
//             NO REFACTORIZAR                   //
///////////////////////////////////////////////////

class inheritance : public HttpConfig, public ServerConfig, public LocationConfig
{
	private:
		void takeHttpValues();
		void takeServerValues();
	public:
		inheritance();
		~inheritance();

        void runInheritance();
};