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

enum DataType
{
    VOID,
    INT,
    FLOAT,
    CHAR,
    STRING
};

class Token
{
    public:
        int type;
        std::string keyword;
};

class Variable
{
    public:
        int type = 0;
        
        int int_value;
        float float_value;
        char char_value;
        std::string string_value;
};

class Function
{
    public:
        std::vector<Token> code;
        std::string return_type;
        std::map<std::string, Variable> args;
        std::map<std::string, Variable> variables;
};

#endif
