#ifndef LOCATIONBLOCKSTRATEGY_HPP
# define LOCATIONBLOCKSTRATEGY_HPP

# include "../IValidationStrategyBlock.hpp"

class LocationBlockStrategy : public IValidationStrategyBlock {

	public:
        LocationBlockStrategy();
		virtual bool validate(std::vector<AConfigBlock*> const &block) const;
};

#endif