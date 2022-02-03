#ifndef DEPENDENCIES_HPP
#define DEPENDENCIES_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>

enum TokenType
{
    INSTRUCTION,    //  0
    LABEL,          //  1
    FUNCTION,       //  2
    VARIABLE,       //  3
    DATA_TYPE,      //  4
    OPPERATOR,      //  5
    null,           //  6
    VALUE,          //  7
    CALL,           //  8
    REFERENCE,      //  9
    START,          // 10
    END             // 11
};

enum DataType
{
    VOID = 0,
    INT = 1,
    FLOAT = 2,
    CHAR = 3,
    STRING = 4
};

class Token
{
    public:
        int type;
        std::string keyword;
};

bool evaluate_conditions(std::vector<Token> args);

class Variable
{
    public:
        int type = VOID;
        
        int int_value = 0;
        float float_value = 0;
        char char_value = ' ';
        std::string string_value = "";
        bool CONSTANT = false;

        std::string name = "";

        void set_type(int t)
        {
            type = t;
        }

        int get_type()
        {
            return type;
        }

        void set_to_keyword(std::string value)
        {
            if (CONSTANT == false)
            {
                switch (type)
                {
                    case STRING:
                        string_value = value;
                        break;
                    case INT:
                        int_value = std::stoi(value);
                        break;
                    case FLOAT:
                        float_value = std::stof(value);
                        break;
                    case CHAR:
                        char_value = value[0];
                        break;
                    default:
                        std::cout << name << ": DATA TYPE NOT SET\n";
                        break;
                }
            }
        }

        void set_to_variable(Variable value)
        {
            if (CONSTANT == false)
            {
                string_value = value.string_value;
                int_value = value.int_value;
                float_value = value.float_value;
                char_value = value.char_value;
            }
        }

        std::string get_as_string()
        {
            switch (type)
            {
                case STRING:
                    return string_value;
                    break;
                case INT:
                    return std::to_string(int_value);
                    break;
                case FLOAT:
                    return std::to_string(float_value);
                    break;
                case CHAR:
                    return std::to_string(char_value);
                    break;
            }
            return "";
        }
};

class Function
{
    public:
        std::vector<Token> code = {};
        int return_type = VOID; // Enum DataType
        std::map<std::string, Variable> args = {};
        std::vector<std::string> args_order = {};
        std::map<std::string, Variable> variables = {};
        std::vector<std::string> variables_order = {};
        std::map<std::string, int> labels = {};

        int code_index = 0;
        int nest_counter = 0;
        std::string name = "";

        bool has_variable(std::string var_name)
        {
            if (std::find(variables_order.begin(), variables_order.end(), var_name) != variables_order.end())
                return true;
            else
                return false;
        }

        bool has_argument(std::string arg_name)
        {
            if (std::find(args_order.begin(), args_order.end(), arg_name) != args_order.end())
                return true;
            else
                return false;
        }
};

#endif
