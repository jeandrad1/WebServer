# include "string"

typedef struct s_errorPage
{
	std::string	targetPage;
	bool		isEqualModifier;
	int			equalModifier;
	int			referencesCount;
}	t_errorPage;

typedef struct s_return
{
	std::string	http;
	int			code;
	bool		returnDirective;
}	t_return;

typedef struct s_limit_except
{	
	bool	POST;
	bool	GET;
	bool	DELETE;
	bool	limitDirective;
}	t_limit_except;

typedef struct s_listen
{
	std::string	ip;
	int			port;
}	t_listen;

typedef struct s_cgi
{
	std::string extension;
	std::string path;
}	t_cgi;