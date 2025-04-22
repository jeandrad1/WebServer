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

typedef struct s_limit_except
{	
	bool 		POST;
	bool 		GET;
	bool 		DELETE;
	bool 		limitDirective;
}   t_limit_except;

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
		t_limit_except				*limit_except;

		virtual ~LocationConfig();
		virtual void printValues(int indent = 0);
};

#endif
