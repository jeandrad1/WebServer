#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include <string>
# include <vector>
# include <cstdlib>
# include <sstream>
# include <map>
# include "IConfig.hpp"

typedef struct s_errorPage
{
    std::string         targetPage;
    bool                isEqualModifier;
    int                 equalModifier;
	int					referencesCount;
}   t_errorPage;

typedef struct s_return
{
    std::string http;
    int			code;
	bool 		returnDirective;
}   t_return;

class LocationConfig : public IConfig
{

	public:
		std::string					locationPath;
		std::string					root;
		std::vector<std::string>	index;
		bool						autoindex;
		long long					clientMaxBodySize;
		t_return					*_return;
        std::map<int, t_errorPage *>  errorPages;
		bool						errorPageDirective;
};

#endif
