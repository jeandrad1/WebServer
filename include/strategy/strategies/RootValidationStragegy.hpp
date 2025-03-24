#ifndef RootValidationStrategy_HPP
# define RootValidationStrategy_HPP

#include "../IValidationStrategy.hpp"

class RootValidationStrategy : public IValidationStrategy
{
    public:
        bool validate(const std::string &value) const;

    private:
        bool hasForbiddenCharacters(const std::string &value) const;
        bool isReservedName(const std::string &value) const;
};

#endif