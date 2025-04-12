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

void LocationBuilder::handleRoot(std::string const &value)
{
    this->location->root = value;
}