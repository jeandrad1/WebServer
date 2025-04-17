#include "ServerConfig.hpp"

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/

int ServerConfig::getListenPort(void)
{
    return (this->listen->port);
}

std::string ServerConfig::getListenIP(void)
{
    return (this->listen->ip);
}
