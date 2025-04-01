#ifndef LOCATIONBLOCK_HPP
# define LOCATIONBLOCK_HPP

# include	<vector>
# include	<string>

# include "AConfigBlock.hpp"

class	LocationBlock : public AConfigBlock {

	private:
		LocationBlock(void);

	public:
		LocationBlock(const std::string &name);
		LocationBlock(const LocationBlock &other);
		virtual	~LocationBlock(void);

		//LocationBlock	&operator=(const LocationBlock &other);

		virtual void	printConfig(int indent = 0) const;
};

#endif
