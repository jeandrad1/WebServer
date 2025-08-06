#ifndef HTTPBLOCK_HPP
# define HTTPBLOCK_HPP

# include "AConfigBlock.hpp"

class	HttpBlock : public AConfigBlock {

	private:
		HttpBlock(void);

	public:
		HttpBlock(const std::string &name);
		HttpBlock(const HttpBlock &other);
		virtual	~HttpBlock(void);

		virtual void	printConfig(int indent = 0) const;
};

#endif
