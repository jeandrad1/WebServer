#ifndef COMPOSITE_HPP
# define COMPOSITE_HPP

#include <fstream>
#include <sstream>
#include <iostream>
#include "../utils/colors.hpp"
#include "../parser/config/AConfigBlock.hpp"
#include "../parser/config/ServerBlock.hpp"
#include "../parser/config/Directive.hpp"
#include "../parser/config/HttpBlock.hpp"
#include "../parser/config/LocationBlock.hpp"

AConfigBlock *createBlock(std::ifstream &filename, AConfigBlock &block);
void lineBuilder(std::ifstream &filename, std::string &line);

#endif