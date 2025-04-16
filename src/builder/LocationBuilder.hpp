#ifndef LOCATIONBUILDER_HPP
#define LOCATIONBUILDER_HPP

#include "LocationConfig.hpp"
#include "IConfigBuilder.hpp"
#include "DirectiveProcessor.hpp"

class LocationBuilder : public IConfigBuilder, public DirectiveProcessor<LocationBuilder>
{
	private:
	LocationConfig	*LocationConfig;
	bool			built;

	public:
	LocationBuilder();
	virtual ~LocationBuilder() {}

	virtual void setDirective(const std::string &key,const std::string &value);
	virtual void addNestedBuilder(IConfigBuilder *child);
	virtual void *build(void);

	void handleRoot(std::string const &value);
	void handleIndex(std::string const &value);
	void handleClient_max_body_size(std::string const &value);
	void handleAutoindex(std::string const &value);
	void handleError_page(std::string const &value);
	void handleReturn(std::string const &value);
};

#endif