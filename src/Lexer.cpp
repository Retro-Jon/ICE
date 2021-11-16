#include "dependencies.hpp"

std::vector<Token> Lex(std::map<std::string, std::string> source_files)
{
    std::vector<Token> tokens;

    for (std::pair<std::string, std::string> file : source_files)
    {
        std::string current;
        bool is_string = false;
        int type;

        for (char c : file.second)
        {
            if (c == '"')
                is_string = !is_string;
            
            if ((c != ' ' && c != '\n' && c != '@' && c != '!' && c != '$' && c != ',' && c != ':' && c != ';' && c != '(' && c != ')' && c != '{' && c != '}' && c != '"') && is_string == false)
                current += c;
            else if (is_string == true && c != '"')
                current += c;
            
            if (is_string == false)
            {
                if (current == "Function")
                {
                    type = FUNCTION;
                    current = "";
                }
                else if (current == "Method")
                {
                    type = METHOD;
                    current = "";
                }

                if (c == ';' || c == '\n' || c == '(' || c == ',' || c == ':')
                {
                    if (current != "")
                    {
                        Token new_token;
                        new_token.keyword = current;
                        new_token.type = type;
                        tokens.push_back(new_token);
                    }
                    current = "";
                    type = INSTRUCTION;
                }

                // Set Type

                if (type == INSTRUCTION)
                {
                    switch (c)
                    {
                        case '$':
                            type = VARIABLE;
                            break;
                        case '@':
                            type = LABEL;
                            break;
                        case ':':
                            type = DATA_TYPE;
                            break;
                        case '+':
                        case '-':
                        case '*':
                        case '/':
                        case '=':
                        case '%':
                            type = OPPERATOR;
                    }
                }
            }
            
        }
    }
    return tokens;
}