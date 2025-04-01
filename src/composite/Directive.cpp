#include "Directive.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

Directive::Directive(void)
{

}

Directive::Directive(const std::string &directive, const std::string &value) : _value(value)
{
	_name = directive;
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
	this->_name = other._name;
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
	std::cout << spaces << this->_name << " " << this->_value << "\n";
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/

std::string    Directive::getValue(void) const
{
	return (this->_value);
}