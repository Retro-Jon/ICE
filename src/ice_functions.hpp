#ifndef ICE_FUNCTIONS_HPP
#define ICE_FUNCTIONS_HPP

#include "file.hpp"

std::vector<Token> Lex(std::map<std::string, std::string> source_files);
int run(std::vector<Token> token_list, bool debug);

#endif
