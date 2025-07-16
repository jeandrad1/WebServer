#ifndef CGIHEADERPARSER_HPP
# define CGIHEADERPARSER_HPP

# define DEFAULT_STATUS_HEADER "HTTP/1.1 200 OK\n"
# define DEFAULT_CONTENT_TYPE_HEADER "Content-Type: text/plain\n"
# define DEFAULT_LOCATION_HEADER "HTTP/1.1 200 OK\n"

# include <string>
# include "../utils/utils.hpp"
# include <map>
# include <set>

class CgiHeaderParser {

    private:
    
        void loadMimeTypes(void);
        void loadErrorCodes(void);
    
    public:
    
        static std::map<int, std::string> error_codes;
        static std::map<std::string, std::string> mime_types;
        static std::set<std::string> known_mime_types;
    
        CgiHeaderParser(void);
        static bool        parseFormatHeader(std::string line);

        static std::string parseStatusHeader(std::string line);
        static std::string parseLocationHeader(std::string line);
        static std::string parseContentTypeHeader(std::string line);
};

#endif