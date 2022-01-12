#include "dependencies.hpp"

bool return_value = false;
bool and_statement = false;
bool or_statement = false;

bool is_num(std::string a)
{
    char c = a[0];
    if((c >= '0' && c <= '9') || c == '-')
        return true;
    
    return false;
}

void set_return_value(bool op_value)
{
    if (and_statement == true)
    {
        if (return_value == true && op_value == true)
        {
            return_value = true;
        } else {
            return_value = false;
        }

        and_statement = false;
    } else if (or_statement == true)
    {
        if (return_value == true || op_value == true)
        {
            return_value = true;
        } else {
            return_value = false;
        }

        or_statement = false;
    } else {
        return_value = op_value;
    }
}

bool evaluate_conditions(std::vector<Token> args)
{
    return_value = false;

    for (int t = 0; t < args.size(); t++)
    {
        Token current_arg = args[t];

        if (current_arg.type == OPPERATOR)
        {
            if (current_arg.keyword == "&&")
            {
                and_statement = true;
            } else if (current_arg.keyword == "||")
            {
                or_statement = true;
            } else if (current_arg.keyword == "==")
            {
                if (args[t - 1].keyword == args[t + 1].keyword)
                    set_return_value(true);
                else set_return_value(false);
            } else if (is_num(args[t - 1].keyword) && is_num(args[t + 1].keyword))
            {
                if (current_arg.keyword == ">")
                {
                    if (std::stof(args[t - 1].keyword) > std::stof(args[t + 1].keyword))
                        set_return_value(true);
                    else set_return_value(false);
                } else if (current_arg.keyword == "<")
                {
                    if (std::stof(args[t - 1].keyword) < std::stof(args[t + 1].keyword))
                        set_return_value(true);
                    else set_return_value(false);
                } else if (current_arg.keyword == ">=")
                {
                    if (std::stof(args[t - 1].keyword) >= std::stof(args[t + 1].keyword))
                        set_return_value(true);
                    else set_return_value(false);
                } else if (current_arg.keyword == "<=")
                {
                    if (std::stof(args[t - 1].keyword) <= std::stof(args[t + 1].keyword))
                        set_return_value(true);
                    else set_return_value(false);
                }
            }
        }
    }
    
    return return_value;
}