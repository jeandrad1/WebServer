#include "../../include/config/LocationBlock.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

LocationBlock::LocationBlock(void)
{

} 

LocationBlock::LocationBlock(const std::string &name) : AConfigBlock(name)
{

}

LocationBlock::LocationBlock(const LocationBlock &other)
{
	*this = other;
}

LocationBlock::~LocationBlock(void)
{

}

/***********************************************************************/
/*                         Operator Overload                           */
/***********************************************************************/


/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

void	LocationBlock::printConfig(int indent) const
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