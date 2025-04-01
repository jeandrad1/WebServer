#ifndef ServerNameStrategy_HPP
#define ServerNameStrategy_HPP

# include "IValidationStrategy.hpp"
# include <cctype>

class ServerNameStrategy : public IValidationStrategy
{
public:
    bool validate(const std::string &value) const;
};

#endif