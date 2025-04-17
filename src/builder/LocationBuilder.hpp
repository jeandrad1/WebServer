#ifndef LOCATIONBUILDER_HPP
#define LOCATIONBUILDER_HPP

#include "LocationConfig.hpp"
#include "IConfigBuilder.hpp"
#include "DirectiveProcessor.hpp"

class LocationBuilder : public IConfigBuilder, public DirectiveProcessor<LocationBuilder>
{
	private:
		LocationConfig	*location;
		bool			built;

	public:
		LocationBuilder();
		~LocationBuilder(){};

		void setDirective(const std::string &key,const std::string &value);
		void addNestedBuilder(IConfigBuilder *child){};
		void *build(void);

		void handleRoot(std::string const &value);
		void handleIndex(std::string const &value);
		void handleClient_max_body_size(std::string const &value);
		void handleAutoindex(std::string const &value);
		void handleError_page(std::string const &value);
		void handleReturn(std::string const &value);
};

#endif
