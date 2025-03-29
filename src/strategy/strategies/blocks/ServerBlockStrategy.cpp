#include "../../../../include/strategy/blocks/ServerBlockStrategy.hpp"
#include "../../../../include/parser/config/Directive.hpp"

ServerBlockStrategy::ServerBlockStrategy()
{
    this->validateBlock.push_back("location");
    this->validateBlock.push_back("listen");
    this->validateBlock.push_back("server_name");
    this->validateBlock.push_back("root");
    this->validateBlock.push_back("index");
    this->validateBlock.push_back("client_max_body_size");
    this->validateBlock.push_back("autoindex");
    this->validateBlock.push_back("error_page");
    this->validateBlock.push_back("return");
}

bool ServerBlockStrategy::validate(std::vector<AConfigBlock*> const &block) const
{
	std::vector<AConfigBlock*>::const_iterator itb = block.begin();
	for(std::vector<AConfigBlock*>::const_iterator ite = block.end(); itb != ite; itb++)
	{
		if (std::find(this->validateBlock.begin(), this->validateBlock.end(), (*itb)->getName()) == this->validateBlock.end())
			return false;
	}
	return true;
}

int main() {
    return 0;
}