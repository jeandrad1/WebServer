#ifndef LOCATIONBUILDER_HPP
#define LOCATIONBUILDER_HPP

#include "LocationConfig.hpp"
#include "IConfigBuilder.hpp"
#include "DirectiveProcessor.hpp"

# define DEFAULT_CLIENT_MAX_BODY_SIZE (1024 * 1024)
# define DEFAULT_AUTOINDEX false

class LocationBuilder : public IConfigBuilder, public DirectiveProcessor<LocationBuilder>
{
	private:
		LocationConfig	*location;
		bool			built;

	public:
		LocationBuilder(const std::string &path);
		~LocationBuilder();

		void setDirective(const std::string &key,const std::string &value);
		void addNestedBuilder(IConfigBuilder *child, AConfigBlock *newBlock);
		void *build(AConfigBlock *locationBlock);
		void setDefaultValues(void);

		void handleRoot(std::string const &value);
		void handleIndex(std::string const &value);
		void handleClientMaxBodySize(std::string const &value);
		void handleAutoindex(std::string const &value);
		void handleErrorPage(std::string const &value);
		void handleReturn(std::string const &value);
};

#endif
