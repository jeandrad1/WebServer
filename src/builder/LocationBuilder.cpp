#include "LocationBuilder.hpp"

LocationBuilder::LocationBuilder() : built(false), location(NULL)
{
    this->registerHandler("root", &LocationBuilder::handleRoot);
    this->registerHandler("index", &LocationBuilder::handleIndex);
    this->registerHandler("client_max_body_size", &LocationBuilder::handleClient_max_body_size);
    this->registerHandler("autoindex", &LocationBuilder::handleAutoindex);
    this->registerHandler("error_page", &LocationBuilder::handleError_page);
    this->registerHandler("return", &LocationBuilder::handleReturn);
}

void    LocationBuilder::handleRoot(const std::string &value)
{
	std::string real_value = value.substr(0, value.size() - 1);
	this->LocationConfig->root = real_value;
}

void LocationBuilder::handleIndex(std::string const &value)
{
    std::string real_value = value.substr(0, value.size() - 1);
    std::vector<std::string> index = split_str(real_value, " ");
    this->LocationConfig->index = index;
}

