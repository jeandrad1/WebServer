#include "LocationBlockStrategy.hpp"

LocationBlockStrategy::LocationBlockStrategy()
{
	this->validDirectives.push_back("root");
	this->validDirectives.push_back("index");
	this->validDirectives.push_back("client_max_body_size");
	this->validDirectives.push_back("autoindex");
	this->validDirectives.push_back("error_page");
	this->validDirectives.push_back("return");
	this->validDirectives.push_back("limit_except");
	this->validDirectives.push_back("cgi");
}

bool	LocationBlockStrategy::validate(std::vector<AConfigBlock*> const &block) const
{
	std::vector<AConfigBlock*>::const_iterator		itb = block.begin();

	for(std::vector<AConfigBlock*>::const_iterator	ite = block.end(); itb != ite; itb++)
	{
		if (std::find(this->validDirectives.begin(), this->validDirectives.end(), (*itb)->getName()) == this->validDirectives.end())
			return (false);
	}
	return (true);
}
