#ifndef STRUCTS_HPP
# define STRUCTS_HPP

# include <string>
# include <vector>


typedef struct s_locationErrorPage
{
    std::string         target;
    std::vector<int> statusCodes;
    bool                isEqualModifier;
    int                 equalModifier;            
}   t_locationErrorPage;


#endif
