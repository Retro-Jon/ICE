#include "dependencies.hpp"
#include "file.hpp"

std::vector<Function> Function_Stack = {};

void error(std::string err)
{
    std::cout << "ERROR:\n" << err << "\n";
    Function_Stack.clear();
    std::vector<Function> tmp = {};
    Function_Stack.swap(tmp);
}

int run(std::vector<Token> token_list, bool debug)
{
    /*
    1. Find functions
    2. Start running at main function
    3. Check token type and handle accordingly
    */

    // Global variables in run()
    std::map<std::string, Function> functions = {};

    // Create functions
    {
        bool in_function_code = false;
        bool in_function_args = false;
        Function current_function;
        Token last_token;
        int current_code_index = 0;

        int bracket_counter = 0;

        for (Token t : token_list)
        {
            if ((t.type != FUNCTION && t.type != DATA_TYPE && t.type != LABEL && t.type != OPPERATOR && t.type != null) || (t.type == DATA_TYPE))
            {
                if (in_function_code == true)
                    current_function.code.push_back(t);
            }
            switch(t.type)
            {
                case FUNCTION:
                {
                    current_function.name = t.keyword;
                    break;
                }
                case DATA_TYPE:
                {
                    int type = 0;
                    if (t.keyword == "void") type = VOID;
                    else if (t.keyword == "int") type = INT;
                    else if (t.keyword == "float") type = FLOAT;
                    else if (t.keyword == "char") type = CHAR;
                    else if (t.keyword == "string") type = STRING;
                    
                    if (in_function_args == false && in_function_code == false)
                    {
                        current_function.return_type = type;
                        in_function_args = true;
                    }
                    else if (in_function_args == true)
                    {
                        current_function.args[last_token.keyword].type = type;
                    }
                    break;
                }
                case VARIABLE:
                {
                    if (in_function_args == true && in_function_code == false)
                    {
                        Variable a;
                        a.name = t.keyword;
                        current_function.args.insert(std::pair<std::string, Variable>(t.keyword, a));
                        current_function.args_order.push_back(t.keyword);
                    }
                    break;
                }
                case START:
                {
                    if (bracket_counter == 0)
                    {
                        in_function_code = true;
                        in_function_args = false;
                        current_function.code.push_back(t);
                    }
                    bracket_counter++;
                    break;
                }
                case END:
                {
                    bracket_counter--;
                    if (bracket_counter == 0)
                    {
                        in_function_code = false;
                        // Store function name and function pair in functions map
                        functions.insert(std::pair<std::string, Function>(current_function.name, current_function));
                        Function f;
                        current_function = f;
                        bracket_counter = 0;
                    }
                    break;
                }
                case LABEL:
                {
                    current_function.labels.insert(std::pair<std::string, int>(t.keyword, current_function.code.size() - 1));
                    break;
                }
                case OPPERATOR:
                {
                    if (in_function_code == true)
                    {
                        if (token_list[current_code_index + 1].type == OPPERATOR)
                        {
                            Token op;
                            op.keyword = t.keyword + token_list[current_code_index + 1].keyword;
                            op.type = OPPERATOR;
                            current_function.code.push_back(op);
                            token_list.erase(token_list.begin() + current_code_index + 1);
                        }
                        else if (token_list[current_code_index - 1].type != OPPERATOR)
                        {
                            current_function.code.push_back(t);
                        }
                    }
                }
            }

            last_token = t;
            current_code_index++;
        }
    }

    if (functions.size() == 0)
    {
        error("Functions not created.");
        return 1;
    }

    // List Functions

    if (debug == true)
    {
        for (std::pair<std::string, Function> f : functions)
        {
            std::cout << "------------------" << "\n";
            std::cout << f.first << "\n";
            std::cout << "\nargs:\n";
            for (std::pair<std::string, Variable> a : f.second.args)
                std::cout << a.first << " : " << a.second.type << "\n";
            
            std::cout << "\ncode:\n";
            for (Token t : f.second.code)
                std::cout << t.keyword << "\n";
        }
    }

    if (debug == true) std::cout << "\nSTART EXECUTION\n" << "\n";

    Function_Stack.push_back(functions["main"]);
    Variable Return_Value;

    // Loop through Function_Stack

    while (Function_Stack.size() > 0)
    {
        int current_function_index = Function_Stack.size() - 1;
        bool execute = true;
        bool last_if_condition_result = true;
        bool last_elif_condition_result = false;

        int execute_change_index = 0;

        if (debug == true) std::cout << "--" << Function_Stack[current_function_index].name << "--\n";
        
        // Loop through current function code

        for (int current_code_index = Function_Stack[current_function_index].code_index; current_code_index < Function_Stack[current_function_index].code.size(); ++current_code_index)
        {
            Token current_instruction = Function_Stack[current_function_index].code[current_code_index];

            if (current_instruction.type == START)
            {
                Function_Stack[current_function_index].nest_counter++;
            }
            else if (current_instruction.type == END)
            {
                Function_Stack[current_function_index].nest_counter--;
                if (execute_change_index == Function_Stack[current_function_index].nest_counter)
                {
                    execute = true;
                }
            }

            if (current_instruction.type == INSTRUCTION && current_instruction.keyword == "else")
            {
                if (last_if_condition_result == false && last_elif_condition_result == false && execute_change_index == Function_Stack[current_function_index].nest_counter)
                {
                    execute = true;
                } else {
                    execute = false;
                }
            } else if (current_instruction.type == INSTRUCTION && current_instruction.keyword == "elif")
            {
                if (last_if_condition_result == false && last_elif_condition_result == false && execute_change_index == Function_Stack[current_function_index].nest_counter)
                {
                    std::vector<Token> conditions;
                    Token T;
                    int t_index = current_code_index + 1;
                    T = Function_Stack[current_function_index].code[t_index];

                    while (T.type != START)
                    {
                        T = Function_Stack[current_function_index].code[t_index];

                        if (T.type != START)
                        {
                            conditions.push_back(T);
                        }
                        
                        t_index++;
                    }

                    for (int i = 0; i < conditions.size(); i++)
                    {
                        if (conditions[i].type == REFERENCE)
                        {
                            if (Function_Stack[current_function_index].has_variable(conditions[i].keyword))
                            {
                                conditions[i].keyword = Function_Stack[current_function_index].variables[conditions[i].keyword].get_as_string();
                            }
                            else if (Function_Stack[current_function_index].has_argument(conditions[i].keyword))
                            {
                                conditions[i].keyword = Function_Stack[current_function_index].args[conditions[i].keyword].get_as_string();
                            }
                        }
                    }
                    last_elif_condition_result = evaluate_conditions(conditions);

                    if (last_elif_condition_result == false)
                    {
                        execute_change_index = Function_Stack[current_function_index].nest_counter;
                        execute = false;
                    }
                } else {
                    execute = false;
                }
            }

            if (execute == true)
            {
                if (debug == true) std::cout << current_instruction.keyword << "\n";

                switch (current_instruction.type)
                {
                    case OPPERATOR: // += -= *= /= =
                    {
                        Token var = Function_Stack[current_function_index].code[current_code_index - 1];
                        Token value = Function_Stack[current_function_index].code[current_code_index + 1];

                        if (var.type == VALUE && current_instruction.keyword != "==" && current_instruction.keyword != "!=" && current_instruction.keyword != ">" && current_instruction.keyword != "<" && current_instruction.keyword != ">=" && current_instruction.keyword != "<=" && current_instruction.keyword != "&&" && current_instruction.keyword != "||")
                        {
                            error("Can't modify a value: " + var.keyword + " in Function: " + Function_Stack[current_function_index].name);
                            return 1;
                        }

                        if (value.type == CALL)
                        {
                            if (Return_Value.type == VOID)
                            {
                                Function_Stack[current_function_index].code_index = current_code_index;
                                Function_Stack[current_function_index].nest_counter = Function_Stack[current_function_index].nest_counter;
                                Function new_function;
                                new_function = functions[Function_Stack[current_function_index].code[current_code_index + 1].keyword];

                                for (int arg = 0; arg < new_function.args_order.size(); arg++)
                                {
                                    Variable current_arg = new_function.args[new_function.args_order[arg]];
                                    Token value = Function_Stack[current_function_index].code[current_code_index + arg + 2];
                                    if (value.type == REFERENCE)
                                    {
                                        if (Function_Stack[current_function_index].has_variable(value.keyword))
                                        {
                                            Variable v = Function_Stack[current_function_index].variables[value.keyword];
                                            value.keyword = v.get_as_string();
                                        } else if (Function_Stack[current_function_index].has_argument(value.keyword))
                                        {
                                            Variable a = Function_Stack[current_function_index].args[value.keyword];
                                            value.keyword = a.get_as_string();
                                        }
                                    }
                                    new_function.args[new_function.args_order[arg]].set_to_keyword(value.keyword);
                                }
                                Function_Stack.push_back(new_function);
                                current_code_index = Function_Stack[current_function_index].code.size();
                                break;
                            } else {
                                value.keyword = Return_Value.get_as_string();
                            }
                        }
                        
                        std::string value2;
                                
                        if (Function_Stack[current_function_index].has_variable(value.keyword))
                        {
                            value2 = Function_Stack[current_function_index].variables[value.keyword].get_as_string();
                        } else if (Function_Stack[current_function_index].has_argument(value.keyword))
                        {
                            value2 = Function_Stack[current_function_index].args[value.keyword].get_as_string();
                        } else {
                            value2 = value.keyword;
                        }

                        if (current_instruction.keyword == "=")
                        {
                            if (Function_Stack[current_function_index].has_variable(var.keyword))
                            {
                                Function_Stack[current_function_index].variables[var.keyword].set_to_keyword(value2);
                            }
                            else if (Function_Stack[current_function_index].has_argument(var.keyword))
                            {
                                // ICE ERROR HANDLING
                                error("Cannot modify argument value: " + var.keyword + "; in Function: " + Function_Stack[current_function_index].name);
                                return 1;
                            }
                        } else if (current_instruction.keyword == "+=")
                        {
                            if (Function_Stack[current_function_index].has_variable(var.keyword))
                            {
                                std::string new_value = "";
                                std::string value1 = Function_Stack[current_function_index].variables[var.keyword].get_as_string();

                                switch (Function_Stack[current_function_index].variables[var.keyword].type)
                                {
                                    case INT:
                                        new_value = std::to_string(std::stoi(value1) + std::stoi(value2));
                                        break;
                                    case FLOAT:
                                        new_value = std::to_string(std::stof(value1) + std::stof(value2));
                                        break;
                                    case STRING:
                                        new_value = value1 + value2;
                                        break;
                                }
                                Function_Stack[current_function_index].variables[var.keyword].set_to_keyword(new_value);
                            }
                        } else if (current_instruction.keyword == "-=")
                        {
                            if (Function_Stack[current_function_index].has_variable(var.keyword))
                            {
                                std::string new_value = "";
                                std::string value1 = Function_Stack[current_function_index].variables[var.keyword].get_as_string();

                                switch (Function_Stack[current_function_index].variables[var.keyword].type)
                                {
                                    case INT:
                                        new_value = std::to_string(std::stoi(value1) - std::stoi(value2));
                                        break;
                                    case FLOAT:
                                        new_value = std::to_string(std::stof(value1) - std::stof(value2));
                                        break;
                                    case STRING:
                                        // ICE ERROR HANDLING
                                        error("Cannot subtract strings at: " + var.keyword + " " + current_instruction.keyword + " " + value.keyword + "; in Function: " + Function_Stack[current_function_index].name);
                                        return 1;
                                }
                                Function_Stack[current_function_index].variables[var.keyword].set_to_keyword(new_value);
                            }
                        } else if (current_instruction.keyword == "*=")
                        {
                            if (Function_Stack[current_function_index].has_variable(var.keyword))
                            {
                                std::string new_value = "";
                                std::string value1 = Function_Stack[current_function_index].variables[var.keyword].get_as_string();

                                switch (Function_Stack[current_function_index].variables[var.keyword].type)
                                {
                                    case INT:
                                        new_value = std::to_string(std::stoi(value1) * std::stoi(value2));
                                        break;
                                    case FLOAT:
                                        new_value = std::to_string(std::stof(value1) * std::stof(value2));
                                        break;
                                    case STRING:
                                        // ICE ERROR HANDLING
                                        error("Cannot multiply strings at: " + var.keyword + " " + current_instruction.keyword + " " + value.keyword + "; in Function: " + Function_Stack[current_function_index].name);
                                        return 1;
                                }
                                Function_Stack[current_function_index].variables[var.keyword].set_to_keyword(new_value);
                            }
                        } else if (current_instruction.keyword == "/=")
                        {
                            if (Function_Stack[current_function_index].has_variable(var.keyword))
                            {
                                std::string new_value = "";
                                std::string value1 = Function_Stack[current_function_index].variables[var.keyword].get_as_string();

                                if (std::stoi(value2) != 0)
                                {
                                    switch (Function_Stack[current_function_index].variables[var.keyword].type)
                                    {
                                        case INT:
                                            new_value = std::to_string(std::stoi(value1) / std::stoi(value2));
                                            break;
                                        case FLOAT:
                                            new_value = std::to_string(std::stof(value1) / std::stof(value2));
                                            break;
                                        case STRING:
                                            // ICE ERROR HANDLING
                                            error("Cannot devide strings at: " + var.keyword + " " + current_instruction.keyword + " " + value.keyword + "; in Function: " + Function_Stack[current_function_index].name);
                                            return 1;
                                    }
                                    Function_Stack[current_function_index].variables[var.keyword].set_to_keyword(new_value);
                                } else {
                                    error("Can't devide by 0");
                                }
                            }
                        }
                        break;
                    }
                    case INSTRUCTION:
                    {
                        if (current_instruction.keyword == "output")
                        {
                            std::string var = Function_Stack[current_function_index].code[current_code_index + 2].keyword;
                            // Check if value is a variable reference
                            if (Function_Stack[current_function_index].code[current_code_index + 2].type == REFERENCE)
                            {
                                if (Function_Stack[current_function_index].has_argument(var))
                                { // Is function argument
                                    var = Function_Stack[current_function_index].args[var].get_as_string();
                                }
                                else if (Function_Stack[current_function_index].has_variable(var))
                                {
                                    var = Function_Stack[current_function_index].variables[var].get_as_string();
                                }
                            }

                            switch (std::stoi(Function_Stack[current_function_index].code[current_code_index + 1].keyword))
                            {
                                case 0:
                                {
                                    std::cout << var;
                                    break;
                                }
                                case 1:
                                {
                                    std::string path = Function_Stack[current_function_index].code[current_code_index + 3].keyword;
                                    // Check if value is a variable reference
                                    if (Function_Stack[current_function_index].code[current_code_index + 3].type == REFERENCE)
                                    {
                                        if (Function_Stack[current_function_index].has_argument(path))
                                        { // Is function argument
                                            path = Function_Stack[current_function_index].args[path].get_as_string();
                                        }
                                        else if (Function_Stack[current_function_index].has_variable(path))
                                        {
                                            path = Function_Stack[current_function_index].variables[path].get_as_string();
                                        }
                                    }
                                    save_string(path, var);
                                    break;
                                }
                            }
                        } else if (current_instruction.keyword == "input")
                        {
                            switch (std::stoi(Function_Stack[current_function_index].code[current_code_index + 1].keyword))
                            {
                                case 0:
                                {
                                    std::string var = Function_Stack[current_function_index].code[current_code_index + 2].keyword;

                                    // Check if value is a variable reference
                                    if (Function_Stack[current_function_index].code[current_code_index + 2].type == REFERENCE)
                                    {
                                        if (Function_Stack[current_function_index].has_variable(var))
                                        {
                                            std::string data = "";
                                            std::getline(std::cin, data);
                                            std::cin.clear();
                                            Function_Stack[current_function_index].variables[var].set_to_keyword(data);
                                        }
                                    }
                                    break;
                                }
                                case 1:
                                {
                                    std::string var = Function_Stack[current_function_index].code[current_code_index + 2].keyword;

                                    std::string path = Function_Stack[current_function_index].code[current_code_index + 3].keyword;
                                    // Check if value is a variable reference
                                    if (Function_Stack[current_function_index].code[current_code_index + 3].type == REFERENCE)
                                    {
                                        if (Function_Stack[current_function_index].has_argument(path))
                                        { // Is function argument
                                            path = Function_Stack[current_function_index].args[path].get_as_string();
                                        }
                                        else if (Function_Stack[current_function_index].has_variable(path))
                                        {
                                            path = Function_Stack[current_function_index].variables[path].get_as_string();
                                        }
                                    }

                                    // Check if value is a variable reference
                                    if (Function_Stack[current_function_index].code[current_code_index + 2].type == REFERENCE)
                                    {
                                        if (Function_Stack[current_function_index].has_variable(var))
                                        {
                                            std::string data = "";
                                            data = load_string(path);
                                            Function_Stack[current_function_index].variables[var].set_to_keyword(data);
                                        }
                                    }
                                    break;
                                }
                            }
                        } else if (current_instruction.keyword == "if")
                        {
                            std::vector<Token> conditions;
                            Token T;
                            int t_index = current_code_index + 1;
                            T = Function_Stack[current_function_index].code[t_index];

                            while (T.type != START)
                            {
                                T = Function_Stack[current_function_index].code[t_index];

                                if (T.type != START)
                                {
                                    conditions.push_back(T);
                                }
                                
                                t_index++;
                            }

                            for (int i = 0; i < conditions.size(); i++)
                            {
                                if (conditions[i].type == REFERENCE)
                                {
                                    if (Function_Stack[current_function_index].has_variable(conditions[i].keyword))
                                    {
                                        conditions[i].keyword = Function_Stack[current_function_index].variables[conditions[i].keyword].get_as_string();
                                    }
                                    else if (Function_Stack[current_function_index].has_argument(conditions[i].keyword))
                                    {
                                        conditions[i].keyword = Function_Stack[current_function_index].args[conditions[i].keyword].get_as_string();
                                    }
                                }
                            }
                            last_if_condition_result = evaluate_conditions(conditions);

                            if (last_if_condition_result == false)
                            {
                                execute_change_index = Function_Stack[current_function_index].nest_counter;
                                execute = false;
                            }

                        } else if (current_instruction.keyword == "return")
                        {
                            Return_Value.type = Function_Stack[current_function_index].return_type;
                            std::string r_keyword = Function_Stack[current_function_index].code[current_code_index + 1].keyword;

                            if (Function_Stack[current_function_index].has_variable(r_keyword))
                            {
                                Return_Value.set_to_keyword(Function_Stack[current_function_index].variables[r_keyword].get_as_string());
                            }
                            else if (Function_Stack[current_function_index].has_argument(r_keyword))
                                Return_Value.set_to_keyword(Function_Stack[current_function_index].args[r_keyword].get_as_string());
                            else
                                Return_Value.set_to_keyword(r_keyword);
                            
                            last_if_condition_result = true;
                            execute_change_index = 0;
                            Function_Stack.pop_back();
                            current_code_index = Function_Stack[current_function_index].code.size();
                            
                        } else if (current_instruction.keyword == "goto")
                        {
                            current_code_index = Function_Stack[current_function_index].labels[Function_Stack[current_function_index].code[current_code_index + 1].keyword];
                        }
                        break;
                    }
                    case VARIABLE:
                    {
                        Variable v;
                        v.name = current_instruction.keyword;
                        Function_Stack[current_function_index].variables[current_instruction.keyword] = v;
                        Function_Stack[current_function_index].variables_order.push_back(current_instruction.keyword);
                    }
                    case DATA_TYPE:
                    {
                        if (current_instruction.keyword == "int")
                            Function_Stack[current_function_index].variables[Function_Stack[current_function_index].code[current_code_index - 1].keyword].set_type(INT);
                        else if (current_instruction.keyword == "float")
                            Function_Stack[current_function_index].variables[Function_Stack[current_function_index].code[current_code_index - 1].keyword].set_type(FLOAT);
                        else if (current_instruction.keyword == "char")
                            Function_Stack[current_function_index].variables[Function_Stack[current_function_index].code[current_code_index - 1].keyword].set_type(CHAR);
                        else if (current_instruction.keyword == "string")
                            Function_Stack[current_function_index].variables[Function_Stack[current_function_index].code[current_code_index - 1].keyword].set_type(STRING);
                        break;
                    }
                    case CALL:
                    {
                        if (Return_Value.type == VOID)
                        {
                            Function_Stack[current_function_index].code_index = current_code_index + 1;
                            Function_Stack[current_function_index].nest_counter = Function_Stack[current_function_index].nest_counter;
                            Function new_function;
                            new_function = functions[current_instruction.keyword];

                            for (int arg = 0; arg < new_function.args_order.size(); arg++)
                            {
                                Variable current_arg = new_function.args[new_function.args_order[arg]];
                                Token value = Function_Stack[current_function_index].code[current_code_index + arg + 1];
                                if (value.type == REFERENCE)
                                {
                                    if (Function_Stack[current_function_index].has_variable(value.keyword))
                                    {
                                        Variable v = Function_Stack[current_function_index].variables[value.keyword];
                                        value.keyword = v.get_as_string();
                                    } else if (Function_Stack[current_function_index].has_argument(value.keyword))
                                    {
                                        Variable a = Function_Stack[current_function_index].args[value.keyword];
                                        value.keyword = a.get_as_string();
                                    }
                                }
                                new_function.args[functions[current_instruction.keyword].args_order[arg]].set_to_keyword(value.keyword);
                            }
                            Function_Stack.push_back(new_function);
                            current_code_index = Function_Stack[current_function_index].code.size();
                        } else {
                            Return_Value.type = VOID;
                        }
                        break;
                    }
                    case END:
                    {
                        if (current_code_index == Function_Stack[current_function_index].code.size() - 1)
                        {
                            last_if_condition_result = true;
                            execute_change_index = 0;
                            Function_Stack.pop_back();
                            current_code_index = Function_Stack[current_function_index].code.size();
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }

    std::vector<Function> tmp;
    std::vector<Function>().swap(tmp);

    if (Return_Value.type == INT)
    {
        return std::stoi(Return_Value.get_as_string());
    }
    else
        return 0;
}