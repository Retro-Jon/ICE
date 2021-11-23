#include "dependencies.hpp"

std::vector<Token> Lex(std::map<std::string, std::string> source_files)
{
    std::vector<Token> tokens;
    std::vector<std::string> variables;
    std::vector<std::string> methods_functions;
    std::string instructions[] = {"output", "input", "return"};

    for (std::pair<std::string, std::string> file : source_files)
    {
        std::string current;
        bool is_string = false;
        int type;

        for (char c : file.second)
        {
            if (c == '"')
                is_string = !is_string;
            
            if ((c != ' ' && c != '\n' && c != '@' && c != '!' && c != '$' && c != '+' && c != '-' && c != '*' && c != '/' && c != '=' && c != ',' && c != ':' && c != ';' && c != '(' && c != ')' && c != '{' && c != '}' && c != '[' && c != ']' && c != '"') && is_string == false)
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
                else if (current == "null")
                {
                    type = null;
                    Token new_token;
                    new_token.keyword = current;
                    new_token.type = type;
                    tokens.push_back(new_token);
                    current = "";
                    type = INSTRUCTION;
                }

                if (c == ';' || c == '\n' || c == '(' || c == '[' || c == ',' || c == ':' || c == '+' || c == '-' || c == '*' || c == '/' || c == '=')
                {
                    if (current != "")
                    {
                        Token new_token;
                        new_token.keyword = current;
                        new_token.type = type;
                        
                        if (std::find(std::begin(instructions), std::end(instructions), new_token.keyword) == std::end(instructions))
                        {
                            if (std::find(methods_functions.begin(), methods_functions.end(), new_token.keyword) == methods_functions.end() && new_token.type == INSTRUCTION)
                                new_token.type = VALUE;

                            if (std::find(methods_functions.begin(), methods_functions.end(), new_token.keyword) != methods_functions.end())
                                new_token.type = CALL;
                            else if (std::find(variables.begin(), variables.end(), new_token.keyword) != variables.end() && new_token.type != VARIABLE)
                                new_token.type = REFERENCE;
                        }
                        
                        tokens.push_back(new_token);
                        switch (new_token.type)
                        {
                            case FUNCTION:
                                methods_functions.push_back(new_token.keyword);
                                break;
                            case VARIABLE:
                                variables.push_back(new_token.keyword);
                            default:
                                break;
                        }
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
                        {
                            Token new_token;
                            new_token.keyword = c;
                            new_token.type = OPPERATOR;
                            tokens.push_back(new_token);
                            break;
                        }
                        case '}':
                        {
                            Token end;
                            end.keyword = "END";
                            end.type = END;
                            tokens.push_back(end);
                            break;
                        }
                        case '{':
                        {
                            Token start;
                            start.keyword = "START";
                            start.type = START;
                            tokens.push_back(start);
                        }
                    }
                }
            }
        }
    }
    return tokens;
}