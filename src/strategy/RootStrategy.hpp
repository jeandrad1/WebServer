#ifndef RootStrategy_HPP
# define RootStrategy_HPP

# include "IValidationStrategy.hpp"
# include <set>
# include <algorithm>

class RootStrategy : public IValidationStrategy
{
    public:
        bool validate(const std::string &value) const;

    private:
        bool hasForbiddenCharacters(const std::string &value) const;
        bool isReservedName(const std::string &value) const;
};

#endif