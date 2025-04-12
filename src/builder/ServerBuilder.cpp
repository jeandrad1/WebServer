#include "ServerBuilder.hpp"

ServerBuilder::ServerBuilder()
{
    registerHandler("listen", &ServerBuilder::handleListen);
    registerHandler("server_name", &ServerBuilder::handleServerName);
    registerHandler("root", &ServerBuilder::handleRoot);
    registerHandler("index", &ServerBuilder::handleIndex);
    registerHandler("client_max_body_size", &ServerBuilder::handleClientMaxBodySize);
    registerHandler("autoindex", &ServerBuilder::handleAutoindex);
    registerHandler("error_page", &ServerBuilder::handleErrorPage);
    registerHandler("return", &ServerBuilder::handleReturn);

}
