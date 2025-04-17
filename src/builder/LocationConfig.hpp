#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

#include <string>
#include <vector>
#include <cstdlib>
#include <sstream>

struct ReturnValues
{
	std::string http;
	int code;
};

typedef struct s_errorPage
{
    std::string         target;
    std::vector<int> 	statusCodes;
    bool                isEqualModifier;
    int                 equalModifier;            
}   t_errorPage;

class LocationConfig
{
	public:
		std::string					locationPath;
		std::string					root;
		std::vector<std::string>	index;
		bool						*autoIndex;
		long long					clientMaxBodySize;
		ReturnValues				*_return;
		std::vector<t_errorPage>    errorPages;
};

#endif