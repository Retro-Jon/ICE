#ifndef DEPENDENCIES_HPP
#define DEPENDENCIES_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>

enum TokenType
{
    INSTRUCTION,
    LABEL,
    FUNCTION,
    VARIABLE,
    DATA_TYPE,
    OPPERATOR,
    null,
    VALUE,
    CALL,
    REFERENCE,
    START,
    END
};

class Token
{
    public:
        int type;
        std::string keyword;
};

class Function
{
    public:
        std::vector<Token> code;
        std::string return_type;
        std::vector<Token> args;
};

#endif
