#include "../../include/config/AConfigBlock.hpp"

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
    
    return (*this);
}

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/


/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/