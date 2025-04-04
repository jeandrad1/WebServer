#include "AConfigBlock.hpp"

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
	for (std::vector<AConfigBlock *>::iterator it = blocks.begin(); it != blocks.end(); ++it)
	{
		if (*it)
			delete *it;
	}
	blocks.clear();
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

AConfigBlock::iterator AConfigBlock::begin(void)
{
	return (this->blocks.begin());
}

AConfigBlock::iterator AConfigBlock::end(void)
{
	return (this->blocks.end());
}

AConfigBlock::const_iterator AConfigBlock::begin(void) const
{
	return (this->blocks.begin());
}

AConfigBlock::const_iterator AConfigBlock::end(void) const
{
	return (this->blocks.end());
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

std::vector<AConfigBlock *> AConfigBlock::getBlocksVector(void)
{
	return (this->blocks);
}
