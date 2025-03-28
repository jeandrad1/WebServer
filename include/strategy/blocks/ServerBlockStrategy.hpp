#ifndef SERVERBLOCKSTRATEGY_HPP
# define SERVERBLOCKSTRATEGY_HPP

# include "../IValidationStrategyBlock.hpp"

class ServerBlockStrategy : public IValidationStrategyBlock {

	public:
		ServerBlockStrategy();
		virtual bool validate(std::vector<AConfigBlock*> const &block) const;
};

#endif