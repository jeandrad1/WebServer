#include "../../include/config/Directive.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

Directive::Directive(void)
{

}

Directive::Directive(const std::string &directive, const std::string &value) : _directive(directive), _value(value)
{

}

Directive::Directive(const Directive &other)
{
	*this = other;
}

Directive::~Directive(void)
{

}

/***********************************************************************/
/*                         Operator Overload                           */
/***********************************************************************/

Directive &Directive::operator=(const Directive &other)
{
	if (this == &other)
		return (*this);
	this->_directive = other._directive;
    this->_value = other._value;
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

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

void	Directive::printConfig(int indent) const
{
	std::string spaces(indent * 2, ' ');
	std::cout << spaces << this->_directive << " " << this->_value << ";\n";
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/

std::string    Directive::getDirective(void) const
{
	return (this->_directive);
}

std::string    Directive::getValue(void) const
{
	return (this->_value);
}