#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>

int main()
{
	std::string body =
		"<!DOCTYPE html>\n"
		"<html lang=\"en\">\n"
		"<head>\n"
		"    <meta charset=\"UTF-8\">\n"
		"    <title>Test-CGI</title>\n"
		"</head>\n"
		"<body>\n"
		"<p>Funciona</p>\n"
		"</body>\n"
		"</html>\n";

    std::stringstream header;
    header	<< "Status: 200 OK\r\n"
			<< "Content-Type: text/html\r\n"
			<< "Content-Length: " << body.length() << "\r\n"
			<< "\r\n";

	std::cout << header.str() << body;
	return (0);
}