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
    METHOD,
    VARIABLE,
    DATA_TYPE,
    OPPERATOR,
    null,
    VALUE,
    CALL,
    REFERENCE
};

class Token
{
    public:
        int type;
        std::string keyword;
};

#endif
