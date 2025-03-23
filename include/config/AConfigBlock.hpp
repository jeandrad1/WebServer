#ifndef ACONFIGBLOCK_HPP
# define ACONFIGBLOCK_HPP

# include <iostream>
# include <vector>
# include <string>
# include <stdexcept>

class AConfigBlock {

	protected:
		std::string					_name;
		std::vector<AConfigBlock *>	blocks;
	
		AConfigBlock(void);
	public:
		AConfigBlock(const std::string &name);
		AConfigBlock(const AConfigBlock &other);
		virtual	~AConfigBlock(void);

		AConfigBlock	&operator=(const AConfigBlock &other);

		virtual void	printConfig(int indent = 0) const = 0;
		virtual void	addBlock(AConfigBlock *newBlock);

		std::string getName();
		AConfigBlock *getBlock(int index);
		AConfigBlock *getBeginBlock();
		AConfigBlock *getEndBlock();
};

#endif
