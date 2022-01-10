#include "dependencies.hpp"
#include "file.hpp"

std::vector<Token> Lex(std::map<std::string, std::string> source_files)
{
    std::vector<Token> tokens;
    std::vector<std::string> variables;
    std::vector<std::string> methods_functions;
    std::string instructions[] = {"output", "input", "return", "if", "else"};

    std::vector<std::string> file_list;
    
    for (std::pair<std::string, std::string>file_name : source_files)
    {
        file_list.push_back(file_name.first);
    }

    int file_count = file_list.size();

    for (file_count > 0; file_count--;)
    {
        std::string current_file = file_list[file_count];
        std::string current;
        bool is_string = false;
        bool in_comment = false;
        bool in_ICE = false;
        bool ICE_FETCH = false;
        int type;
        bool escape_sequence = false;

        for (char c : source_files[current_file])
        {
            if (c == '"')
                is_string = !is_string;
            
            if ((c != ' ' && c != '\n' && c != '@' && c != '!' && c != '$' && c != '#' && c != '>' && c != '<' && c != '+' && c != '-' && c != '*' && c != '/' && c != '=' && c != ',' && c != ':' && c != ';' && c != '(' && c != ')' && c != '{' && c != '}' && c != '[' && c != ']' && c != '"') && is_string == false && in_comment == false)
                current += c;
            else if (is_string == true && c != '"' && in_comment == false)
            {
                if (c == '\\' && escape_sequence == false)
                    escape_sequence = true;
                else if (escape_sequence == true)
                {
                    char seq;
                    switch (c)
                    {
                        case 'n':
                            seq = '\n';
                            break;
                        case '0':
                            seq = '\0';
                            break;
                    }
                    current += seq;
                    escape_sequence = false;
                }
                else if (escape_sequence == false)
                {
                    current += c;
                }
            }
            else if (c == '#')
                in_comment = !in_comment;
            else if (current == "ICE" && in_comment == false && is_string == false)
            {
                in_ICE = true;
                current = "";
            }

            if (in_ICE == true)
            {
                if (c == ';')
                {
                    if (ICE_FETCH == true)
                    {
                        std::string dir = current_file;

                        for (int dir_c = dir.size(); dir_c > 0; dir_c--)
                        {
                            if (dir[dir_c] == '\\')
                                break;
                            else
                            {
                                dir.erase(dir_c);
                            }
                        }
                        if (std::find(file_list.begin(), file_list.end(), (dir + current)) == file_list.end())
                        {
                            source_files.insert(std::pair<std::string, std::string>(dir + current, load_string(dir + current)));
                            file_list.push_back(dir + current);
                            file_count += 2;
                            break;
                        }
                        ICE_FETCH = false;
                    }
                    current = "";
                    in_ICE = false;
                }
                else if (current == "Fetch")
                {
                    current = "";
                    ICE_FETCH = true;
                }
            }

            if (is_string == false && in_comment == false && in_ICE == false)
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

                if (c == ';' || c == '\n' || c == '(' || c == '{' || c == '[' || c == ',' || c == ':' || c == '+' || c == '-' || c == '*' || c == '/' || c == '=')
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
                            break;
                        }
                    }
                }
            }
        }
    }
    return tokens;
}