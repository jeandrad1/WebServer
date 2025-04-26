#include "ServerToLocationInheritance.hpp"

void ServerToLocationInheritance::inherit(LocationConfig &location, ServerConfig &server) {
	this->configureClientMaxBodySize(location, server);
	this->configureErrorPage(location, server);
	this->configureRoot(location, server);
    this->configureIndex(location, server);
}

void ServerToLocationInheritance::configureClientMaxBodySize(LocationConfig &location, ServerConfig &server)
{
	if (location.clientMaxBodySize == -1 && server.clientMaxBodySize != -1)
		location.clientMaxBodySize = server.clientMaxBodySize;
	else if (location.clientMaxBodySize == -1 && server.clientMaxBodySize == -1)
		location.clientMaxBodySize = DEFAULT_CLIENT_MAX_BODY_SIZE;
	if (server.clientMaxBodySize == -1)
		server.clientMaxBodySize = DEFAULT_CLIENT_MAX_BODY_SIZE;
}
void ServerToLocationInheritance::configureErrorPage(LocationConfig &location, const ServerConfig &server)
{
	if (server.errorPageDirective && !location.errorPageDirective) {
		location.errorPageDirective = true;
		location.errorPages = server.errorPages;
	}
}

void ServerToLocationInheritance::configureRoot(LocationConfig &location, ServerConfig &server)
{
	if (location.root == "-1" && server.root != "-1")
		location.root = server.root;
	else if (location.root == "-1" && server.root == "-1")
		location.root = DEFAULT_ROOT;
	if (server.root == "-1")
		server.root = DEFAULT_ROOT;
}

void ServerToLocationInheritance::configureIndex(LocationConfig &location, ServerConfig &server)
{
	if (location.index[0] == " " && server.index[0] != " ")
	{
		location.index.clear();
		location.index = server.index;
	}
	else if (location.index[0] == " " && server.index[0] == " ")
	{
		location.index.clear();
		location.index.push_back("index.html");
		location.index.push_back("index.htm");
	}
	if (server.index[0] == " ")
	{
		server.index.clear();
		server.index.push_back("index.html");
		server.index.push_back("index.htm");
	}
}

void ServerToLocationInheritance::noLocations(ServerConfig &server)
{
	if(server.root == "-1")
		server.root = DEFAULT_ROOT;
	if (server.index[0] == " ")
	{
		server.index.clear();
		server.index.push_back("index.html");
		server.index.push_back("index.htm");
	}
}