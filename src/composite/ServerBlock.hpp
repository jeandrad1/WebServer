#ifndef SERVERBLOCK_HPP
# define SERVERBLOCK_HPP

# include "AConfigBlock.hpp"

class	ServerBlock : public AConfigBlock 
{

	private:
		ServerBlock(void);

	public:
		ServerBlock(const std::string &name);
		ServerBlock(const ServerBlock &other);
		virtual	~ServerBlock(void);

		virtual void	printConfig(int indent = 0) const;
};

#endif
