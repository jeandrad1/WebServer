#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

#include <string>
#include <vector>
#include <cstdlib>
#include <sstream>

typedef struct s_errorPage
{
    std::string         target;
    std::vector<int>    statusCodes;
    bool                isEqualModifier;
    int                 equalModifier;            
}   t_errorPage;

typedef struct s_return
{
    std::string http;
    int code;
}   t_return;

class LocationConfig {

	public:
		std::string					locationPath;
		std::string					root;
		std::vector<std::string>	index;
		bool						autoindex;
		long long					clientMaxBodySize;
		t_return					*_return;
		std::vector<t_errorPage *>    errorPages;
};

#endif
