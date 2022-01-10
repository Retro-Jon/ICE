#include "dependencies.hpp"

bool evaluate_conditions(std::vector<Token> args)
{
    for (Token t : args)
    {
        //std::cout << t.keyword << std::endl;
    }
    if (args[0].keyword == "true")
        return true;
    
    return false;
}