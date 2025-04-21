#include "LocationBuilder.hpp"
#include <cstdlib>

std::vector<std::string> split_str(const std::string &str, const std::string &delimiter);

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

LocationBuilder::LocationBuilder(const std::string &path) : built(false), location(new LocationConfig())
{
    std::string newPath;

    if (path.size() > 8)
    {
        newPath = path.substr(8, path.size());
        newPath.erase(0, newPath.find_first_not_of(" \t"));
        newPath.erase(newPath.find_last_not_of(" \t") + 1);
    }
    else
        newPath = "";

    this->location->locationPath = newPath;
    setDefaultValues();
    

    this->registerHandler("root", &LocationBuilder::handleRoot);
    this->registerHandler("index", &LocationBuilder::handleIndex);
    this->registerHandler("client_max_body_size", &LocationBuilder::handleClientMaxBodySize);
    this->registerHandler("autoindex", &LocationBuilder::handleAutoindex);
    this->registerHandler("error_page", &LocationBuilder::handleErrorPage);
    this->registerHandler("return", &LocationBuilder::handleReturn);
}

LocationBuilder::~LocationBuilder()
{

}

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

void    LocationBuilder::setDirective(const std::string &name, const std::string &value)
{
    dispatchDirective(name, value);
}

void    LocationBuilder::addNestedBuilder(IConfigBuilder *child, AConfigBlock *newBlock)
{
    LocationConfig *newLocation = static_cast<LocationConfig *>(child->build(newBlock));
    this->built = false;
}

IConfig *LocationBuilder::build(AConfigBlock *locationBlock)
{
    AConfigBlock::iterator blockItEnd = locationBlock->end();
    for (AConfigBlock::iterator blockIt = locationBlock->begin(); blockIt != blockItEnd; blockIt++)
    {
        if (Directive *directive = dynamic_cast<Directive *>(*blockIt))
        this->dispatchDirective(directive->getName(), directive->getValue());
    }
    return (this->location);
}

void    LocationBuilder::setDefaultValues(void)
{
    this->location->_return = new t_return;

    this->location->autoindex = DEFAULT_AUTOINDEX;
	this->location->clientMaxBodySize = DEFAULT_CLIENT_MAX_BODY_SIZE;
    this->location->root = DEFAULT_ROOT;

    this->location->index.push_back("index.html");
    this->location->index.push_back("index.htm");

    this->location->_return->returnDirective = false;
    this->location->errorPageDirective = false;
}

/***********************************************************************/
/*                          Handle Functions                           */
/***********************************************************************/

void    LocationBuilder::handleRoot(const std::string &value)
{
	std::string real_value = value.substr(0, value.size() - 1);
	this->location->root = real_value;
}

void LocationBuilder::handleIndex(std::string const &value)
{
	std::string real_value = value.substr(0, value.size() - 1);
    std::vector<std::string> index = split_str(real_value, " ");
    this->location->index = index;
}

void LocationBuilder::handleClientMaxBodySize(std::string const &value)
{
	if (value.empty())
		return ;

	std::istringstream	iss(value);
	unsigned long		maxBodySize = 0;
	char				suffix = '\0';

	iss >> maxBodySize;
	iss >> suffix;
	switch (tolower(suffix))
	{
		case 'k':
			this->location->clientMaxBodySize = maxBodySize * 1024;
			break ;
		case 'm':
			this->location->clientMaxBodySize = maxBodySize * 1024 * 1024;
			break ;
		case 'g':
			this->location->clientMaxBodySize = maxBodySize * 1024 * 1024 * 1024;
			break ;
		case '\0':
			this->location->clientMaxBodySize = maxBodySize;
	}
}

void    LocationBuilder::handleAutoindex(const std::string &value)
{
    std::string real_value = value.substr(0, value.size() - 1);
    if (real_value == "off")
        this->location->autoindex = false;
    else
        this->location->autoindex = true;
}

void    LocationBuilder::handleErrorPage(const std::string &value)
{
    std::istringstream iss(value);
    std::vector<std::string> values;
    std::string info;
    t_errorPage *errorPage = new t_errorPage;
    
    this->location->errorPageDirective = true;

    if (value.empty())
    return ;
    
    while (!iss.eof())
    {
        iss >> info;
        values.push_back(info);
    }
    
    std::vector<std::string>::iterator ite = values.end();
    ite--;
    std::string target = *ite;

    errorPage->targetPage = target.substr(0, target.size() - 1);
    errorPage->isEqualModifier = false;
    errorPage->equalModifier = 0;
    errorPage->referencesCount = 0;

    ite--;
    if (!(*ite).find('='))
    {
        errorPage->isEqualModifier = true;
        errorPage->equalModifier = std::atol((*ite).substr(1, (*ite).size()).c_str());
        ite--;
    }
    ite++;
    for (std::vector<std::string>::iterator it = values.begin(); it != ite; it++)
    {
        errorPage->referencesCount++;
        this->location->errorPages[std::atol((*it).c_str())] = errorPage;
    }
}

void    LocationBuilder::handleReturn(const std::string &value)
{
    if (this->location->_return->returnDirective == true)
    {
        std::cout << "Error in builder because of Return encountered twice, should throw exception?";
        return ;
    }

    this->location->_return->returnDirective = true;

    std::string real_value = value.substr(0, value.size() - 1);
    int http_pos = real_value.find("h");

    if(http_pos != std::string::npos)
    {
        this->location->_return->http = real_value.substr(http_pos, value.size());
        std::string port_str = real_value.substr(0, http_pos);
        this->location->_return->code = std::atoi(port_str.c_str());
    }
    else
    {
        std::string true_value = real_value.substr(0, std::string::npos);
        this->location->_return->code = std::atoi(true_value.c_str());
    }
}
