#include "ServerBlockStrategy.hpp"

ServerBlockStrategy::ServerBlockStrategy()
{
	this->validDirectives.push_back("listen");
	this->validDirectives.push_back("server_name");
	this->validDirectives.push_back("root");
	this->validDirectives.push_back("index");
	this->validDirectives.push_back("client_max_body_size");
	this->validDirectives.push_back("autoindex");
	this->validDirectives.push_back("error_page");
	this->validDirectives.push_back("return");
	this->validDirectives.push_back("cgi");
}

std::string	trim(const std::string &str);

bool	validateLocation(const std::string &str)
{
	if (str.empty())
		return (false);

	std::string path = trim(str);

	if (path[0] != '/')
		return (false);
	
	if (path.find("//") != std::string::npos)
		return (false);

	for (size_t i = 0; i < path.size(); ++i)
	{
		char c = path[i];

		if (isalnum(c) || c == '/' || c == '_' || c == '-' || c == '.' || c == '%')
			continue;
		
		return (false);
	}
	return (true);
}

bool	ServerBlockStrategy::validate(std::vector<AConfigBlock*> const &block) const
{
	std::vector<AConfigBlock*>::const_iterator  itb = block.begin();

	for (std::vector<AConfigBlock*>::const_iterator	ite = block.end(); itb != ite; itb++)
	{
		if (std::find(this->validDirectives.begin(), this->validDirectives.end(), (*itb)->getName()) == this->validDirectives.end())
		{
			std::string	name = (*itb)->getName();
			if (name.find("location") != std::string::npos
				&& validateLocation(name.substr(8, name.size())))
			{
				return (true);
			}
			return (false);
		}
	}
	return (true);
}