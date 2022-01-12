#include "dependencies.hpp"
#include "file.hpp"

struct Function_Stack_Data
{
    std::string name;
    int code_index;
    int nest_counter;
};

std::vector<Function_Stack_Data> Function_Stack;

void error(std::string err)
{
    std::cout << "ERROR:\n" << err << "\n";
    Function_Stack.clear();
    std::vector<Function_Stack_Data> tmp;
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
    std::map<std::string, Function> functions;

    // Create functions
    {
        bool in_function_code = false;
        bool in_function_args = false;
        Function current_function;
        std::string current_function_name = "";
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
                    current_function_name = t.keyword;
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
                        functions.insert(std::pair<std::string, Function>(current_function_name, current_function));
                        current_function_name = "";
                        Function f;
                        f.args.clear();
                        f.variables.clear();
                        f.code.clear();
                        f.labels.clear();
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

    Function_Stack_Data main;
    main.name = "main";
    main.code_index = 0;
    main.nest_counter = 0;
    Function_Stack.push_back(main);
    Variable Return_Value;

    // Loop through Function_Stack

    while (Function_Stack.size() > 0)
    {
        int current_function_index = Function_Stack.size() - 1;
        Function_Stack_Data current_function = Function_Stack[current_function_index];
        int nest_counter = current_function.nest_counter;
        bool execute = true;
        bool last_condition_result = true;

        int execute_change_index = 0;

        if (debug == true) std::cout << "--" << current_function.name << "--\n";

        // Loop through current function code

        for (int index = current_function.code_index; index < functions[current_function.name].code.size(); ++index)
        {
            Token current_instruction = functions[current_function.name].code[index];

            if (debug == true) std::cout << current_instruction.keyword << "\n";

            if (current_instruction.type == START)
            {
                nest_counter++;
            }
            else if (current_instruction.type == END)
            {
                nest_counter--;
                if (execute_change_index == nest_counter)
                {
                    execute = true;
                }
            }

            if (current_instruction.type == INSTRUCTION && current_instruction.keyword == "else")
            {
                execute_change_index = nest_counter;
                execute = !last_condition_result;
            }

            if (execute == true)
            {
                switch (current_instruction.type)
                {
                    case OPPERATOR:
                    {
                        Token var = functions[current_function.name].code[index - 1];
                        Token value = functions[current_function.name].code[index + 1];

                        if (var.type == VALUE && current_instruction.keyword != "==" && current_instruction.keyword != "!=" && current_instruction.keyword != ">" && current_instruction.keyword != "<" && current_instruction.keyword != ">=" && current_instruction.keyword != "<=" && current_instruction.keyword != "&&" && current_instruction.keyword != "||")
                        {
                            error("Can't modify a value: " + var.keyword + " in Function: " + current_function.name);
                            return 1;
                        }

                        if (value.type == CALL)
                        {
                            if (Return_Value.type == VOID)
                            {
                                Function_Stack[current_function_index].code_index = index;
                                Function_Stack[current_function_index].nest_counter = nest_counter;
                                Function_Stack_Data new_function;
                                new_function.name = functions[current_function.name].code[index + 1].keyword;
                                new_function.code_index = 0;
                                new_function.nest_counter = 0;
                                Function_Stack.push_back(new_function);

                                for (int arg = 0; arg < functions[new_function.name].args_order.size(); arg++)
                                {
                                    Token current_value = functions[current_function.name].code[index + arg + 2];

                                    if (current_value.type == REFERENCE)
                                    {
                                        if (functions[current_function.name].has_variable(current_value.keyword))
                                        {
                                            current_value.keyword = functions[current_function.name].variables[current_value.keyword].get_as_string();
                                        } else if (functions[current_function.name].has_argument(current_value.keyword))
                                        {
                                            current_value.keyword = functions[current_function.name].args[current_value.keyword].get_as_string();
                                        }
                                    }
                                    
                                    functions[new_function.name].args[functions[new_function.name].args_order[arg]].set_to_keyword(current_value.keyword);
                                }
                                index = functions[current_function.name].code.size();
                                break;
                            } else {
                                value.keyword = Return_Value.get_as_string();
                            }
                        }
                        if (current_instruction.keyword == "=")
                        {
                            if (functions[current_function.name].has_variable(var.keyword))
                            {
                                if (functions[current_function.name].code[index + 1].type == REFERENCE)
                                {
                                    if (functions[current_function.name].has_variable(value.keyword))
                                        functions[current_function.name].variables[var.keyword].set_to_variable(functions[current_function.name].variables[value.keyword]);
                                    else if (functions[current_function.name].has_argument(value.keyword))
                                        functions[current_function.name].variables[var.keyword].set_to_variable(functions[current_function.name].args[value.keyword]);
                                } else {
                                    functions[current_function.name].variables[var.keyword].set_to_keyword(value.keyword);
                                }
                            }
                            else if (functions[current_function.name].has_argument(var.keyword))
                            {
                                // ICE ERROR HANDLING
                                error("Cannot modify argument value: " + var.keyword + "; in Function: " + current_function.name);
                                return 1;
                            }
                        } else if (current_instruction.keyword == "+=")
                        {
                            if (functions[current_function.name].has_variable(var.keyword))
                            {
                                std::string new_value = "";
                                std::string value1 = functions[current_function.name].variables[var.keyword].get_as_string();
                                std::string value2;
                                
                                if (functions[current_function.name].has_variable(value.keyword))
                                {
                                    value2 = functions[current_function.name].variables[value.keyword].get_as_string();
                                } else if (functions[current_function.name].has_argument(value.keyword))
                                {
                                    value2 = functions[current_function.name].args[value.keyword].get_as_string();
                                } else {
                                    value2 = value.keyword;
                                }

                                switch (functions[current_function.name].variables[var.keyword].type)
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
                                functions[current_function.name].variables[var.keyword].set_to_keyword(new_value);
                            }
                        } else if (current_instruction.keyword == "-=")
                        {
                            if (functions[current_function.name].has_variable(var.keyword))
                            {
                                std::string new_value = "";
                                std::string value1 = functions[current_function.name].variables[var.keyword].get_as_string();
                                std::string value2;
                                
                                if (functions[current_function.name].has_variable(value.keyword))
                                {
                                    value2 = functions[current_function.name].variables[value.keyword].get_as_string();
                                } else if (functions[current_function.name].has_argument(value.keyword))
                                {
                                    value2 = functions[current_function.name].args[value.keyword].get_as_string();
                                } else {
                                    value2 = value.keyword;
                                }

                                switch (functions[current_function.name].variables[var.keyword].type)
                                {
                                    case INT:
                                        new_value = std::to_string(std::stoi(value1) - std::stoi(value2));
                                        break;
                                    case FLOAT:
                                        new_value = std::to_string(std::stof(value1) - std::stof(value2));
                                        break;
                                    case STRING:
                                        // ICE ERROR HANDLING
                                        error("Cannot subtract strings at: " + var.keyword + " " + current_instruction.keyword + " " + value.keyword + "; in Function: " + current_function.name);
                                        return 1;
                                }
                                functions[current_function.name].variables[var.keyword].set_to_keyword(new_value);
                            }
                        } else if (current_instruction.keyword == "*=")
                        {
                            if (functions[current_function.name].has_variable(var.keyword))
                            {
                                std::string new_value = "";
                                std::string value1 = functions[current_function.name].variables[var.keyword].get_as_string();
                                std::string value2;
                                
                                if (functions[current_function.name].has_variable(value.keyword))
                                {
                                    value2 = functions[current_function.name].variables[value.keyword].get_as_string();
                                } else if (functions[current_function.name].has_argument(value.keyword))
                                {
                                    value2 = functions[current_function.name].args[value.keyword].get_as_string();
                                } else {
                                    value2 = value.keyword;
                                }

                                switch (functions[current_function.name].variables[var.keyword].type)
                                {
                                    case INT:
                                        new_value = std::to_string(std::stoi(value1) * std::stoi(value2));
                                        break;
                                    case FLOAT:
                                        new_value = std::to_string(std::stof(value1) * std::stof(value2));
                                        break;
                                    case STRING:
                                        // ICE ERROR HANDLING
                                        error("Cannot multiply strings at: " + var.keyword + " " + current_instruction.keyword + " " + value.keyword + "; in Function: " + current_function.name);
                                        return 1;
                                }
                                functions[current_function.name].variables[var.keyword].set_to_keyword(new_value);
                            }
                        } else if (current_instruction.keyword == "/=")
                        {
                            if (functions[current_function.name].has_variable(var.keyword))
                            {
                                std::string new_value = "";
                                std::string value1 = functions[current_function.name].variables[var.keyword].get_as_string();
                                std::string value2;
                                
                                if (functions[current_function.name].has_variable(value.keyword))
                                {
                                    value2 = functions[current_function.name].variables[value.keyword].get_as_string();
                                } else if (functions[current_function.name].has_argument(value.keyword))
                                {
                                    value2 = functions[current_function.name].args[value.keyword].get_as_string();
                                } else {
                                    value2 = value.keyword;
                                }

                                switch (functions[current_function.name].variables[var.keyword].type)
                                {
                                    case INT:
                                        new_value = std::to_string(std::stoi(value1) / std::stoi(value2));
                                        break;
                                    case FLOAT:
                                        new_value = std::to_string(std::stof(value1) / std::stof(value2));
                                        break;
                                    case STRING:
                                        // ICE ERROR HANDLING
                                        error("Cannot devide strings at: " + var.keyword + " " + current_instruction.keyword + " " + value.keyword + "; in Function: " + current_function.name);
                                        return 1;
                                }
                                functions[current_function.name].variables[var.keyword].set_to_keyword(new_value);
                            }
                        }
                        break;
                    }
                    case INSTRUCTION:
                    {
                        if (current_instruction.keyword == "output")
                        {
                            Function func;
                            func = functions[current_function.name];
                            std::string var = func.code[index + 2].keyword;
                            // Check if value is a variable reference
                            if (func.code[index + 2].type == REFERENCE)
                            {
                                if (func.has_argument(var))
                                { // Is function argument
                                    var = func.args[var].get_as_string();
                                }
                                else if (func.has_variable(var))
                                {
                                    var = func.variables[var].get_as_string();
                                }
                            }

                            switch (std::stoi(functions[current_function.name].code[index + 1].keyword))
                            {
                                case 0:
                                {
                                    std::cout << var;
                                    break;
                                }
                                case 1:
                                {
                                    std::string path = func.code[index + 3].keyword;
                                    // Check if value is a variable reference
                                    if (func.code[index + 3].type == REFERENCE)
                                    {
                                        if (func.has_argument(path))
                                        { // Is function argument
                                            path = func.args[path].get_as_string();
                                        }
                                        else if (func.has_variable(path))
                                        {
                                            path = func.variables[path].get_as_string();
                                        }
                                    }
                                    save_string(path, var);
                                    break;
                                }
                            }
                        } else if (current_instruction.keyword == "input")
                        {
                            switch (std::stoi(functions[current_function.name].code[index + 1].keyword))
                            {
                                case 0:
                                {
                                    Function func;
                                    func = functions[current_function.name];
                                    std::string var = func.code[index + 2].keyword;

                                    // Check if value is a variable reference
                                    if (func.code[index + 2].type == REFERENCE)
                                    {
                                        if (func.has_variable(var))
                                        {
                                            std::string data = "";
                                            std::getline(std::cin, data);
                                            functions[current_function.name].variables[var].set_to_keyword(data);
                                        }
                                    }
                                    break;
                                }
                                case 1:
                                {
                                    Function func;
                                    func = functions[current_function.name];
                                    std::string var = func.code[index + 2].keyword;

                                    std::string path = func.code[index + 3].keyword;
                                    // Check if value is a variable reference
                                    if (func.code[index + 3].type == REFERENCE)
                                    {
                                        if (func.has_argument(path))
                                        { // Is function argument
                                            path = func.args[path].get_as_string();
                                        }
                                        else if (func.has_variable(path))
                                        {
                                            path = func.variables[path].get_as_string();
                                        }
                                    }

                                    // Check if value is a variable reference
                                    if (func.code[index + 2].type == REFERENCE)
                                    {
                                        if (func.has_variable(var))
                                        {
                                            std::string data = "";
                                            data = load_string(path);
                                            functions[current_function.name].variables[var].set_to_keyword(data);
                                        }
                                    }
                                    break;
                                }
                            }
                        } else if (current_instruction.keyword == "if")
                        {
                            std::vector<Token> conditions;
                            Token T;
                            int t_index = index + 1;
                            T = functions[current_function.name].code[t_index];

                            while (T.type != START)
                            {
                                T = functions[current_function.name].code[t_index];

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
                                    if (functions[current_function.name].has_variable(conditions[i].keyword))
                                    {
                                        conditions[i].keyword = functions[current_function.name].variables[conditions[i].keyword].get_as_string();
                                    }
                                    else if (functions[current_function.name].has_argument(conditions[i].keyword))
                                    {
                                        conditions[i].keyword = functions[current_function.name].args[conditions[i].keyword].get_as_string();
                                    }
                                }
                            }
                            last_condition_result = evaluate_conditions(conditions);

                            if (last_condition_result == false)
                            {
                                execute_change_index = nest_counter;
                                execute = false;
                            }
                        } else if (current_instruction.keyword == "return")
                        {
                            Return_Value.type = functions[current_function.name].return_type;
                            std::string r_keyword = functions[current_function.name].code[index + 1].keyword;

                            if (functions[current_function.name].has_variable(r_keyword))
                            {
                                Return_Value.set_to_keyword(functions[current_function.name].variables[r_keyword].get_as_string());
                            }
                            else if (functions[current_function.name].has_argument(r_keyword))
                                Return_Value.set_to_keyword(functions[current_function.name].args[r_keyword].get_as_string());
                            else
                                Return_Value.set_to_keyword(r_keyword);
                        } else if (current_instruction.keyword == "goto")
                        {
                            index = functions[current_function.name].labels[functions[current_function.name].code[index + 1].keyword];
                        }
                        break;
                    }
                    case VARIABLE:
                    {
                        Variable v;
                        functions[current_function.name].variables.insert(std::pair<std::string, Variable>(current_instruction.keyword, v));
                        functions[current_function.name].variables_order.push_back(current_instruction.keyword);
                    }
                    case DATA_TYPE:
                    {
                        if (current_instruction.keyword == "int")
                            functions[current_function.name].variables[functions[current_function.name].code[index - 1].keyword].type = INT;
                        else if (current_instruction.keyword == "float")
                            functions[current_function.name].variables[functions[current_function.name].code[index - 1].keyword].type = FLOAT;
                        else if (current_instruction.keyword == "char")
                            functions[current_function.name].variables[functions[current_function.name].code[index - 1].keyword].type = CHAR;
                        else if (current_instruction.keyword == "string")
                            functions[current_function.name].variables[functions[current_function.name].code[index - 1].keyword].type = STRING;
                        break;
                    }
                    case CALL:
                    {
                        if (Return_Value.type == VOID)
                        {
                            Function_Stack[current_function_index].code_index = index + 1;
                            Function_Stack[current_function_index].nest_counter = nest_counter;
                            Function_Stack_Data new_function;
                            new_function.name = current_instruction.keyword;
                            new_function.code_index = 0;
                            new_function.nest_counter = 0;
                            Function_Stack.push_back(new_function);

                            for (int arg = 0; arg < functions[current_instruction.keyword].args_order.size(); arg++)
                            {
                                Variable current_arg = functions[new_function.name].args[functions[current_instruction.keyword].args_order[arg]];
                                Token value = functions[current_function.name].code[index + arg + 1];
                                
                                if (value.type == REFERENCE)
                                {
                                    if (functions[current_function.name].has_variable(value.keyword))
                                    {
                                        Variable v = functions[current_function.name].variables[value.keyword];
                                        value.keyword = v.get_as_string();
                                    } else if (functions[current_function.name].has_argument(value.keyword))
                                    {
                                        Variable a = functions[current_function.name].args[value.keyword];
                                        value.keyword = a.get_as_string();
                                    }
                                }
                                
                                functions[new_function.name].args[functions[current_instruction.keyword].args_order[arg]].set_to_keyword(value.keyword);
                            }
                            index = functions[current_function.name].code.size();
                        } else {
                            Return_Value.type = VOID;
                        }
                        break;
                    }
                    case END:
                    {
                        if (index == functions[current_function.name].code.size() - 1)
                        {
                            last_condition_result = true;
                            execute_change_index = 0;
                            Function_Stack.pop_back();
                            index = functions[current_function.name].code.size();
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }

    std::vector<Function_Stack_Data> tmp;
    std::vector<Function_Stack_Data>().swap(tmp);

    if (Return_Value.type == INT)
    {
        return std::stoi(Return_Value.get_as_string());
    }
    else
        return 0;
}