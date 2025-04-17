#ifndef ICONFIGBUILDER_HPP
# define ICONFIGBUILDER_HPP

# include <string>
# include "../composite/AConfigBlock.hpp"
# include "../composite/Directive.hpp"
# include "BuilderDefaultMacros.hpp"

class IConfigBuilder {
    
    public:
        virtual         ~IConfigBuilder() {}

        virtual void    setDirective(const std::string &key,const std::string &value) = 0;
        virtual void    addNestedBuilder(IConfigBuilder *child, AConfigBlock *newBlock) = 0;
        virtual void    *build(AConfigBlock *block) = 0;
        virtual void    setDefaultValues(void) = 0;
};

#endif