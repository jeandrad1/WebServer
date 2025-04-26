#include "HttpToServerInheritance.hpp"

void HttpToServerInheritance::inherit(ServerConfig &server, HttpConfig &http) {

	this->configureClientMaxBodySize(server, http);

    if (http.errorPageDirective && !server.errorPageDirective) {
        server.errorPageDirective = true;
        server.errorPages = http.errorPages;
    }
}

void HttpToServerInheritance::configureClientMaxBodySize(ServerConfig &server, HttpConfig &http)
{
	if (server.clientMaxBodySize == -1 && http.clientMaxBodySize != -1)
	server.clientMaxBodySize = http.clientMaxBodySize;
	else if (server.clientMaxBodySize == -1 && http.clientMaxBodySize == -1)
		server.clientMaxBodySize = DEFAULT_CLIENT_MAX_BODY_SIZE;
	if (http.clientMaxBodySize == -1)
		http.clientMaxBodySize = DEFAULT_CLIENT_MAX_BODY_SIZE;
}

void configureErrorPage(ServerConfig &server, HttpConfig const &http)
{
	if (http.errorPageDirective && !server.errorPageDirective) {
		server.errorPageDirective = true;
		server.errorPages = http.errorPages;
	}
}