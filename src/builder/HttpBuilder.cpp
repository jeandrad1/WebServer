#include "HttpBuilder.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

HttpBuilder::HttpBuilder() : built(false)
{
    registerHandler("client_max_body_size", &HttpBuilder::handleClientMaxBodySize);
    registerHandler("error_page", &HttpBuilder::handleErrorPage);
}

/***********************************************************************/
/*                         Operator Overload                           */
/***********************************************************************/

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

void    HttpBuilder::addNestedBuilder(IConfigBuilder *child)
{
    ServerConfig *newServer = static_cast<ServerConfig *>(child->build());
    http->servers.push_back(newServer);
}

void    HttpBuilder::handleClientMaxBodySize(const std::string &value)
{
    if (!value)
        return ;
    
    std::istringstream iss(value);
    unsigned long   maxBodySize = 0;
    char            suffix = '\0';

    iss >> maxBodySize;
    iss >> suffix;
    switch (tolower(suffix))
    {
        case 'k':
            this->http->clientMaxBodySize = maxBodySize * 1024;
            break ;
        case 'm':
            this->http->clientMaxBodySize = maxBodySize * 1024 * 1024;
            break ;
        case 'g':
            this->http->clientMaxBodySize = maxBodySize * 1024 * 1024 * 1024;
            break ;
        case '\0':
            this->http->clientMaxBodySize = maxBodySize;
    }
}

void    HttpBuilder::handleErrorPage(const std::string &value)
{
    if (value.empty())
        return ;

    std::istringstream iss(value);
    std::vector<std::string> values;
    std::string info;
    t_errorPage errorPage;

    while (!iss.eof())
    {
        iss >> info;
        values.push_back(info);
    }
    std::vector<std::string>::iterator ite = values.end();
    ite--;
    std::string target = *ite;
    errorPage.target = target.substr(0, target.size() - 1);
    for (std::vector<std::string>::iterator it = values.begin(); it != ite; it++)
    {
        errorPage.statusCodes.push_back(*it);
    }
    this->http->errorPages.push_back(errorPage);
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/
