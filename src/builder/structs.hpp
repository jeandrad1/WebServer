#ifndef STRUCTS_HPP
# define STRUCTS_HPP

# include <string>
# include <vector>

typedef struct s_errorPage
{
    std::string target;
    std::vector<int> statusCodes;
}   t_errorPage;

#endif