#include <fstream>
#include <sstream>
#include <iostream>
#include "../include/config/AConfigBlock.hpp"
#include "../include/config/ServerBlock.hpp"
#include "../include/config/Directive.hpp"
#include "../include/config/HttpBlock.hpp"
#include "../include/config/LocationBlock.hpp"
#include "../include/utils/colors.hpp"
#include <algorithm>

bool check_chars(std::string value)
{
    long count = 0;
    for (char c : value)
    {
        if (c > 31 && c < 127)
            count++;
    }
    return count != 0;
}

bool index_strategy(Directive *index)
{
    // check if null
    if (index == NULL)
        return false;
    // check there are some spaces and letters
    if (check_chars == false)
        return false;
        
    // Trim the ends

    // If one word do a .html end check

    // Else split and do a loop of the function before

}