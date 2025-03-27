#ifndef HTTPBLOCKSTRATEGY_HPP
# define HTTPBLOCKSTRATEGY_HPP

# include "../IValidationStrategyBlock.hpp"

class HttpBlockStrategy : public IValidationStrategyBlock {
	public:
		HttpBlockStrategy();
		virtual bool validate(std::vector<AConfigBlock*> const &block) const;
};

#endif