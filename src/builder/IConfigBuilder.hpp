#ifndef ICONFIGBUILDER_HPP
# define ICONFIGBUILDER_HPP

# include <string>

class IConfigBuilder {
    
    public:
        virtual         ~IConfigBuilder() {}

        virtual void    setDirective(const std::string &key,const std::string &value) = 0;
        virtual void    addNestedBuilder(IConfigBuilder *child) = 0;
        virtual void    *build(void) = 0;
};

#endif