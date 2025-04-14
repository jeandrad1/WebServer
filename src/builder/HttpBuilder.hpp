#ifndef HTTPBUILDER_HPP
# define HTTPBUILDER_HPP

# include "IConfigBuilder.hpp"
# include "DirectiveProcessor.hpp"
# include "HttpConfig.hpp"
# include "structs.hpp"

class HttpBuilder : public IConfigBuilder, public DirectiveProcessor<HttpBuilder> {

    private:
        HttpConfig  *http;
        bool        built;

    public:
        HttpBuilder();
        ~HttpBuilder();

        void setDirective(const std::string &name, const std::string &value);
        void addNestedBuilder(IConfigBuilder *child);
        void *build();

        void handleErrorPage(const std::string &value);
        void handleClientMaxBodySize(const std::string &value);
};

#endif