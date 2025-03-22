#ifndef ACONFIGBLOCK_HPP
# define ACONFIGBLOCK_HPP

# include <iostream>
# include <vector>
# include <string>

class AConfigBlock {

	protected:
		std::string					_name;
		
		AConfigBlock(void);
	public:
		std::vector<AConfigBlock *>	blocks;
		AConfigBlock(const std::string &name);
		AConfigBlock(const AConfigBlock &other);
		virtual	~AConfigBlock(void);

		AConfigBlock	&operator=(const AConfigBlock &other);
		AConfigBlock	*operator[](int index);

		AConfigBlock	*getBlock(int index)
		{
			return (this->blocks[index]);
		}

		virtual void	printConfig(int indent = 0) const = 0;
		virtual void	addBlock(AConfigBlock *newBlock);
};

#endif
