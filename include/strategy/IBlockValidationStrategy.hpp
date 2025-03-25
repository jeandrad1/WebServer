#ifndef IBLOCKVALIDATIONSTRATEGY_HPP
#define IBLOCKVALIDATIONSTRATEGY_HPP

#include "../parser/config/AConfigBlock.hpp"

class IBlockValidationStrategy {

    public:
        virtual ~IBlockValidationStrategy() {}
        virtual bool validate(AConfigBlock &block) const = 0;
};

#endif