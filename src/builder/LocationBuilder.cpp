#include "LocationBuilder.hpp"

std::vector<std::string>	split_str(const std::string &str, const std::string &delimiter);

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

LocationBuilder::LocationBuilder(const std::string &path) : location(new LocationConfig()), built(false)
{
	std::string	newPath;

	if (path.size() > 8)
	{
		newPath = path.substr(8, path.size());
		newPath.erase(0, newPath.find_first_not_of(" \t"));
		newPath.erase(newPath.find_last_not_of(" \t") + 1);
	}
	else
		newPath = "";

	this->location->setLocationPath(newPath);
	setDefaultValues();

	this->registerHandler("root", &LocationBuilder::handleRoot);
	this->registerHandler("index", &LocationBuilder::handleIndex);
	this->registerHandler("client_max_body_size", &LocationBuilder::handleClientMaxBodySize);
	this->registerHandler("autoindex", &LocationBuilder::handleAutoindex);
	this->registerHandler("error_page", &LocationBuilder::handleErrorPage);
	this->registerHandler("return", &LocationBuilder::handleReturn);
	this->registerHandler("limit_except", &LocationBuilder::handleLimitExcept);
	this->registerHandler("cgi", &LocationBuilder::handleCGI);
}

LocationBuilder::~LocationBuilder()
{

}

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

void	LocationBuilder::setDirective(const std::string &name, const std::string &value)
{
	dispatchDirective(name, value);
}

void	LocationBuilder::addNestedBuilder(IConfigBuilder *child, AConfigBlock *newBlock)
{
	(void) child;
	(void) newBlock;
}

IConfig	*LocationBuilder::build(AConfigBlock *locationBlock)
{
	if (this->built == true)
		return (NULL);

	AConfigBlock::iterator		blockItEnd = locationBlock->end();
	for (AConfigBlock::iterator	blockIt = locationBlock->begin(); blockIt != blockItEnd; blockIt++)
	{
		if (Directive *directive = dynamic_cast<Directive *>(*blockIt))
			this->dispatchDirective(directive->getName(), directive->getValue());
	}
	this->built = true;
	return (this->location);
}

void	LocationBuilder::setDefaultValues(void)
{
	this->location->setClientMaxBodySize(-1);
	
	t_cgi *cgi = new t_cgi;

	std::string r_set = "-1";
    this->location->setRoot(r_set);
	
	this->location->setAutoIndex(DEFAULT_AUTOINDEX);

	std::vector<std::string> index_default;
	index_default.push_back(" ");
    this->location->setIndex(index_default);
	
	t_return *r_temp = new t_return;
	r_temp->returnDirective = false;
	this->location->setReturn(r_temp);
	
	this->location->setErrorPageDirective(false);

	t_limit_except *t_temp = new t_limit_except;
	t_temp->limitDirective = false;
	t_temp->POST = true;
	t_temp->GET = true;
	t_temp->DELETE = false;
	this->location->setLimitExcept(t_temp);

	this->location->cgiDirective = false;
	cgi->extension = DEFAULT_CGI_EXTENSION;
	cgi->path = DEFAULT_CGI_PATH;
	this->location->cgi.push_back(cgi);
}

/***********************************************************************/
/*                          Handle Functions                           */
/***********************************************************************/

void   	LocationBuilder::handleRoot(const std::string &value)
{
	std::string	real_value = value.substr(0, value.size() - 1);
	this->location->setRoot(real_value);
}

void	LocationBuilder::handleIndex(std::string const &value)
{
	std::string					real_value = value.substr(0, value.size() - 1);
	std::vector<std::string>	index = split_str(real_value, " ");
	this->location->setIndex(index);
}

void	LocationBuilder::handleClientMaxBodySize(std::string const &value)
{
	if (value.empty())
		return ;

	std::string			real_value = value.substr(0, value.size() - 1);
	std::istringstream	iss(real_value);
	unsigned long		maxBodySize = 0;
	char				suffix = '\0';

	iss >> maxBodySize;
	iss >> suffix;

	switch (tolower(suffix))
	{
		case 'k':
			this->location->setClientMaxBodySize(maxBodySize * 1024);
			break ;
		case 'm':
			this->location->setClientMaxBodySize(maxBodySize * 1024 * 1024);
			break ;
		case 'g':
			this->location->setClientMaxBodySize(maxBodySize * 1024 * 1024 * 1024);
			break ;
		case '\0':
			this->location->setClientMaxBodySize(maxBodySize);
			break ;
		default :
			std::cout<<"Impossible value found"<<std::endl;
			return ;
	}
	if (this->location->getClientMaxBodySize() > (1024 * 1024 * 1024))
		this->location->setClientMaxBodySize(1024 * 1024 * 1024);
}

void	LocationBuilder::handleAutoindex(const std::string &value)
{
	std::string	real_value = value.substr(0, value.size() - 1);
	if (real_value == "off")
		this->location->setAutoIndex(false);
	else
		this->location->setAutoIndex(true);
}

void	LocationBuilder::handleErrorPage(const std::string &value)
{
	std::istringstream			iss(value);
	std::vector<std::string>	values;
	std::string					info;
	t_errorPage					*errorPage = new t_errorPage;

	this->location->setErrorPageDirective(true);

	if (value.empty())
		return ;

	while (!iss.eof())
	{
		iss >> info;
		values.push_back(info);
	}

	std::vector<std::string>::iterator	ite = values.end();
	ite--;
	std::string							target = *ite;

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
	std::map<int, t_errorPage *> t_errorPages;
	for (std::vector<std::string>::iterator it = values.begin(); it != ite; it++)
	{
		errorPage->referencesCount++;
		t_errorPages[std::atol((*it).c_str())] = errorPage;
	}
	this->location->setErrorPages(t_errorPages);
}

void	LocationBuilder::handleReturn(const std::string &value)
{
	if (this->location->getReturn()->returnDirective == true)
	{
		std::cout << "Error in builder because of Return encountered twice, should throw exception?";
		return ;
	}

	this->location->setReturnDirective(true);

	std::string	real_value = value.substr(0, value.size() - 1);
	size_t http_pos = real_value.find("http");

	if (http_pos != std::string::npos)
	{
		std::string t_http = real_value.substr(http_pos, value.size());
		this->location->setReturnHttp(t_http);
		std::string	port_str = real_value.substr(0, http_pos);
		this->location->setReturnCode(std::atoi(port_str.c_str()));
	}
	else
	{
		std::string	true_value = real_value.substr(0, std::string::npos);
		this->location->setReturnCode(std::atoi(true_value.c_str()));
	}
}

void	LocationBuilder::handleLimitExcept(const std::string &value)
{
	if (this->location->getLimitExcept()->limitDirective == true)
	{
		std::cout << "Error in builder because of limmit_except encountered twice, should throw exception?";
		return ;
	}

	this->location->setLimitExceptLimitDirective(true);

	std::string					real_value = value.substr(0, value.size() - 1);
	std::vector<std::string>	values =split_str(real_value," ");

	this->location->setLimitExceptGet(false);
	this->location->setLimitExceptPost(false);

	for (size_t	i = 0; i < values.size(); i++)
	{
		if(values[i] == "POST" )
			this->location->setLimitExceptPost(true);
		else if(values[i] == "GET" )
			this->location->setLimitExceptGet(true);
		else if(values[i] == "DELETE" )
			this->location->setLimitExceptDelete(true);
	}
}

void	LocationBuilder::handleCGI(std::string const &value)
{
	if (this->location->cgiDirective == false)
	{
		delete this->location->cgi[0];
		this->location->cgi.clear();
	}
	this->location->cgiDirective = true;

	t_cgi	*cgi = new t_cgi;

	std::istringstream	iss(value);

	std::string extension;
	std::string path;

	iss >> extension;
	iss >> path;

	cgi->extension = extension;
	cgi->path = path.substr(0, path.size() - 1);
	this->location->cgi.push_back(cgi);
}