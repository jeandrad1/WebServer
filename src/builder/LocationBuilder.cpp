#include "LocationBuilder.hpp"
#include <cstdlib>

std::vector<std::string> split_str(const std::string &str, const std::string &delimiter);

void    LocationBuilder::setDirective(const std::string &name, const std::string &value)
{
    dispatchDirective(name, value);
}

LocationBuilder::LocationBuilder() : built(false), locationConfig(new LocationConfig())
{
    this->registerHandler("root", &LocationBuilder::handleRoot);
    this->registerHandler("index", &LocationBuilder::handleIndex);
    this->registerHandler("client_max_body_size", &LocationBuilder::handleClient_max_body_size);
    this->registerHandler("autoindex", &LocationBuilder::handleAutoindex);
    this->registerHandler("error_page", &LocationBuilder::handleError_page);
    this->registerHandler("return", &LocationBuilder::handleReturn);
}

void *LocationBuilder::build(void)
{
	if(this->locationConfig->root.empty())
		this->locationConfig->root = "/";
	if(this->locationConfig->index.empty())
        this->locationConfig->index.push_back("index.html");
	if (!this->locationConfig->clientmaxbodysize)
		this->locationConfig->clientmaxbodysize = 1048576;
	if (this->locationConfig->autoindex == NULL)
		this->locationConfig->autoindex = 0;
	if (this->locationConfig->_return->code == -1)
		this->locationConfig->_return->code = 200;
	if (this->locationConfig->_return->http.empty())
		this->locationConfig->_return->http = "example.com";
	if (this->locationConfig->errorPages.empty())
	{
		t_errorPage error;
		error.target = "";
		error.isEqualModifier = false;
		error.equalModifier = 0;
	}
    return this->locationConfig;
}

void    LocationBuilder::handleRoot(const std::string &value)
{
	std::string real_value = value.substr(0, value.size() - 1);
	this->locationConfig->root = real_value;
}

void LocationBuilder::handleIndex(std::string const &value)
{
	std::string real_value = value.substr(0, value.size() - 1);
    std::vector<std::string> index = split_str(real_value, " ");
    this->locationConfig->index = index;
}

void LocationBuilder::handleClient_max_body_size(std::string const &value)
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
			this->locationConfig->clientmaxbodysize = maxBodySize * 1024;
			break ;
		case 'm':
			this->locationConfig->clientmaxbodysize = maxBodySize * 1024 * 1024;
			break ;
		case 'g':
			this->locationConfig->clientmaxbodysize = maxBodySize * 1024 * 1024 * 1024;
			break ;
		case '\0':
			this->locationConfig->clientmaxbodysize = maxBodySize;
	}
}

void    LocationBuilder::handleAutoindex(const std::string &value)
{
    std::string real_value = value.substr(0, value.size() - 1);
    if (real_value == "off")
    *(this->locationConfig->autoindex) = false;
    else
        *(this->locationConfig->autoindex) = true;
}

void    LocationBuilder::handleError_page(const std::string &value)
{
    std::istringstream iss(value);
    std::vector<std::string> values;
    std::string info;
    t_errorPage errorPage;
    
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

    errorPage.target = target.substr(0, target.size() - 1);
    errorPage.isEqualModifier = false;
    errorPage.equalModifier = 0;

    for (std::vector<std::string>::iterator it = values.begin(); it != ite; it++)
    {
        if ((*it).find('='))
        {
            errorPage.isEqualModifier = true;
            errorPage.equalModifier = std::atol((*it).substr(1, (*it).size()).c_str());
            break ;
        }
        errorPage.statusCodes.push_back(std::atol((*it).c_str()));
    }
    this->locationConfig->errorPages.push_back(errorPage);
}

void    LocationBuilder::handleReturn(const std::string &value)
{
    std::string real_value = value.substr(0, value.size() - 1);
    int http_pos = real_value.find("h");

    if(http_pos != std::string::npos)
    {
        this->locationConfig->_return->http = real_value.substr(http_pos, value.size());
        std::string port_str = real_value.substr(0, http_pos);
        this->locationConfig->_return->code = std::atoi(port_str.c_str());
    }
    else
    {
        std::string true_value = real_value.substr(0, std::string::npos);
        this->locationConfig->_return->code = std::atoi(true_value.c_str());
    }
}
