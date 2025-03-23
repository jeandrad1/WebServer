#ifndef COMPOSITE_HPP
# define COMPOSITE_HPP

#include <fstream>
#include <sstream>
#include <iostream>
#include "../utils/colors.hpp"
#include "../config/AConfigBlock.hpp"
#include "../config/ServerBlock.hpp"
#include "../config/Directive.hpp"
#include "../config/HttpBlock.hpp"
#include "../config/LocationBlock.hpp"

AConfigBlock *createBlock(std::ifstream &filename, AConfigBlock &block);
void lineBuilder(std::ifstream &filename, std::string &line);

#endif