#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP
#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include <vector>


class HttpResponse
{
    private:
        int statusCode;
        std::string statusMessage;
        std::map<std::string, std::string> headers;
        std::vector<unsigned char> body;
		bool isBinary;

    public:
        void setStatus(int code, const std::string& message);
        void setHeader(const std::string& key, const std::string& value);
        std::string buildResponse() const;
		
		void setBinaryBody(const std::vector<unsigned char>& data); // binary
		void setTextBody(const std::string& data); // text
		const std::vector<unsigned char>& getBodyBinary() const;
		std::string getBody() const;

		int getStatusCode() const;
        std::string getStatusMessage() const;
        const std::map<std::string, std::string>& getHeaders() const;

		bool bodyIsBinary() const;       

};

#endif