#include "../../include/config/HttpBlock.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

HttpBlock::HttpBlock(void)
{

}

HttpBlock::HttpBlock(const std::string &name) : AConfigBlock(name)
{

}

HttpBlock::HttpBlock(const HttpBlock &other)
{
	*this = other;
}

HttpBlock::~HttpBlock(void)
{

}

/***********************************************************************/
/*                         Operator Overload                           */
/***********************************************************************/


/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

void	HttpBlock::printConfig(int indent) const
{
	std::string spaces(indent * 2, ' ');
	std::cout << spaces << _name << " {\n";
	for (size_t i = 0; i < blocks.size(); ++i)
	{
		blocks[i]->printConfig(indent + 1);
	}
	std::cout << spaces << "}\n";
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/