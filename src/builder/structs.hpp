#ifndef STRUCTS_HPP
# define STRUCTS_HPP

# include <string>
# include <vector>


typedef struct s_errorPage
{
    std::string         target;
    std::vector<int>    statusCodes;
    bool                isEqualModifier;
    int                 equalModifier;            
}   t_errorPage;


#endif
