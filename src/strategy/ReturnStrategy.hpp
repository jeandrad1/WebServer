#ifndef ReturnStrategy_HPP
# define ReturnStrategy_HPP

#include "../IValidationStrategy.hpp"

class ReturnStrategy : public IValidationStrategy
{
    public:
        bool validate(const std::string &value) const;
};

#endif