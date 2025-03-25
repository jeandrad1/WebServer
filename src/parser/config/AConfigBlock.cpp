#include "../../../include/parser/config/AConfigBlock.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

AConfigBlock::AConfigBlock(void)
{
}

AConfigBlock::AConfigBlock(const std::string &name) : _name(name)
{
}

AConfigBlock::AConfigBlock(const AConfigBlock &other)
{
	*this = other;
}

AConfigBlock::~AConfigBlock(void)
{
}

/***********************************************************************/
/*                         Operator Overload                           */
/***********************************************************************/

AConfigBlock &AConfigBlock::operator=(const AConfigBlock &other)
{
	if (this == &other)
		return (*this);
	this->_name = other._name;
	if (!blocks.empty())
	{
		std::vector<AConfigBlock *>::iterator ite = this->blocks.end();
		for (std::vector<AConfigBlock *>::iterator it = this->blocks.begin(); it != ite; ++it)
		{
			delete *it;
			it = this->blocks.erase(it);
		}
	}
	this->blocks = other.blocks;
	return (*this);
}

iterator AConfigBlock::begin(void)
{
	return (this->blocks.begin());
}

iterator AConfigBlock::end(void)
{
	return (this->blocks.end());
}

iterator AConfigBlock::cbegin(void)
{
	return (this->blocks.cbegin());
}

iterator AConfigBlock::cend(void)
{
	return (this->blocks.cend());
}

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

void AConfigBlock::addBlock(AConfigBlock *newBlock)
{
	this->blocks.push_back(newBlock);
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/

std::string AConfigBlock::getName()
{
	return (this->_name);
}

AConfigBlock *AConfigBlock::getBlock(int index)
{
	if (index >= this->blocks.size())
		throw(std::out_of_range("Index exceeds blocks size"));
	std::vector<AConfigBlock *>::iterator it = this->blocks.begin();
	for (int i = 0; i != index; i++)
		it++;
	return (*it);
}

AConfigBlock *AConfigBlock::getBeginBlock()
{
	return (*this->blocks.begin());
}

AConfigBlock *AConfigBlock::getEndBlock()
{
	return (*this->blocks.end());
}