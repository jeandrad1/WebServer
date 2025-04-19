#ifndef ICONFIG_HPP
#define ICONFIG_HPP

class IConfig
{
    public:
        virtual ~IConfig() {}
        virtual void printValues() = 0;
};

#endif
