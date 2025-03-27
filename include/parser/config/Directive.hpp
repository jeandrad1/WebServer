#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# include <iostream>
# include <vector>
# include <string>

# include "AConfigBlock.hpp"

class Directive : public AConfigBlock {

	private:
		std::string	_value;
	
		Directive(void);
	public:
		Directive(const std::string &directive, const std::string &value);
		Directive(const Directive &other);
		virtual	~Directive(void);

		Directive	&operator=(const Directive &other);

		std::string	getValue(void) const;
		std::string	getDirective(void) const;

		virtual void	printConfig(int indent = 0) const;
		//virtual void	addBlock(Directive *newBlock);
};

#endif
