#include "../../../include/parser/config/ServerBlock.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

ServerBlock::ServerBlock(void)
{

}

ServerBlock::ServerBlock(const std::string &name) : AConfigBlock(name)
{

}

ServerBlock::ServerBlock(const ServerBlock &other)
{
	*this = other;
}

ServerBlock::~ServerBlock(void)
{

}

/***********************************************************************/
/*                         Operator Overload                           */
/***********************************************************************/


/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

void	ServerBlock::printConfig(int indent) const
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