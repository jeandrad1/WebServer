#include "HttpBlockStrategy.hpp"

HttpBlockStrategy::HttpBlockStrategy()
{
	this->validateBlock.push_back("client_max_body_size");
    this->validateBlock.push_back("error_page");
}

bool HttpBlockStrategy::validate(std::vector<AConfigBlock*> const &block) const
{
	std::vector<AConfigBlock*>::const_iterator itb = block.begin();
	for(std::vector<AConfigBlock*>::const_iterator ite = block.end(); itb != ite; itb++)
	{
		if (std::find(this->validateBlock.begin(), this->validateBlock.end(), (*itb)->getName()) == this->validateBlock.end())
			return false;
	}
	return true;
}
