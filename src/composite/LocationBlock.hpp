#ifndef LOCATIONBLOCK_HPP
# define LOCATIONBLOCK_HPP

# include "AConfigBlock.hpp"

class	LocationBlock : public AConfigBlock {

	private:
		LocationBlock(void);

	public:
		LocationBlock(const std::string &name);
		LocationBlock(const LocationBlock &other);
		virtual	~LocationBlock(void);

		virtual void	printConfig(int indent = 0) const;
};

#endif
