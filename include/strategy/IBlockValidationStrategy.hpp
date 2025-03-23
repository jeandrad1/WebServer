#ifndef IBLOCKVALIDATIONSTRATEGY_HPP
#define IBLOCKVALIDATIONSTRATEGY_HPP

#include <string>
#include <iostream>

class IValidationStrategy
{

public:
    virtual ~IBlockValidationStrategy() {}
    virtual bool validate(AConfigBlock &block) const = 0;
};

#endif