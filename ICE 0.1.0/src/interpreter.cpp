#include "dependencies.hpp"

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

        for (Token t : token_list)
        {
            if ((t.type != FUNCTION && t.type != DATA_TYPE && t.type != LABEL && t.type != null) || (t.type == DATA_TYPE && in_function_code == true))
            {
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
                    
                    if (in_function_args == false)
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
                    if (in_function_args == true)
                    {
                        Variable a;
                        current_function.args.insert(std::pair<std::string, Variable>(t.keyword, a));
                        current_function.args_order.push_back(t.keyword);
                    }
                    break;
                }
                case START:
                {
                    in_function_code = true;
                    in_function_args = false;
                    break;
                }
                case END:
                {
                    in_function_code = false;
                    // Store function name and function pair in functions map
                    functions.insert(std::pair<std::string, Function>(current_function_name, current_function));
                    current_function_name = "";
                    Function f;
                    current_function = f;
                    break;
                }
                case LABEL:
                {
                    current_function.labels.insert(std::pair<std::string, int>(t.keyword, current_code_index));
                    break;
                }
            }

            last_token = t;
        }
    }

    // List Functions

    if (debug == true)
    {
        for (std::pair<std::string, Function> f : functions)
        {
            std::cout << "------------------" << std::endl;
            std::cout << f.first << std::endl;
            std::cout << "\nargs:\n";
            for (std::pair<std::string, Variable> a : f.second.args)
                std::cout << a.first << " : " << a.second.type << std::endl;
            
            std::cout << "\nvariables:\n";
            for (std::pair<std::string, Variable> v : f.second.variables)
                std::cout << v.first << std::endl;
            
            std::cout << "\ncode:\n";
            for (Token t : f.second.code)
                std::cout << t.keyword << std::endl;
        }
    }

    if (debug == true) std::cout << "\nSTART EXECUTION\n" << std::endl;

    struct Function_Stack_Data
    {
        std::string name;
        int code_index;
    };

    std::vector<Function_Stack_Data> Function_Stack;
    Function_Stack_Data main;
    main.name = "main";
    main.code_index = 0;
    Function_Stack.push_back(main);

    // Loop through Function_Stack

    while (Function_Stack.size() > 0)
    {
        Function_Stack_Data current_function = Function_Stack[Function_Stack.size() - 1];
        int current_function_index = Function_Stack.size() - 1;
        if (debug == true)
        {
            std::cout << "--" << current_function.name << "--" << std::endl;
        }

        // Loop through current function code

        for (int index = current_function.code_index; index < functions[current_function.name].code.size(); ++index)
        {
            Token current_instruction = functions[current_function.name].code[index];

            if (debug == true) std::cout << current_instruction.keyword << std::endl;

            switch (current_instruction.type)
            {
                case OPPERATOR:
                {
                    if (current_instruction.keyword == "=")
                    {
                        Variable var = functions[current_function.name].variables[functions[current_function.name].code[index - 1].keyword];
                        std::string value = functions[current_function.name].code[index + 1].keyword;

                        if (functions[current_function.name].code[index + 1].type == REFERENCE)
                        {
                            if (var.type == STRING)
                                value = functions[current_function.name].variables[value].string_value;
                            else if (var.type == INT)
                                value = std::to_string(functions[current_function.name].variables[value].int_value);
                            else if (var.type == FLOAT)
                                value = std::to_string(functions[current_function.name].variables[value].float_value);
                            else if (var.type == CHAR)
                                value = std::to_string(functions[current_function.name].variables[value].char_value);
                        }

                        if (var.type == STRING)
                            var.string_value = value;
                        else if (var.type == INT)
                            var.int_value = std::stoi(value);
                        else if (var.type == FLOAT)
                            var.float_value = std::stof(value);
                        else if (var.type == CHAR)
                            var.char_value = value[0];
                        
                        functions[current_function.name].variables[functions[current_function.name].code[index - 1].keyword] = var;
                    }
                    break;
                }
                case INSTRUCTION:
                {
                    if (current_instruction.keyword == "output")
                    {
                        switch (std::stoi(functions[current_function.name].code[index + 1].keyword))
                        {
                            case 0:
                                Function func;
                                func = functions[current_function.name];
                                std::string value = func.code[index + 2].keyword;

                                // Check if value is a variable reference
                                if (func.code[index + 2].type == REFERENCE)
                                {
                                    if (std::find(func.args_order.begin(), func.args_order.end(), value) != func.args_order.end())
                                    { // Is function argument
                                        switch (func.args[value].type)
                                        {
                                            case VOID:
                                                value = "VOID";
                                                break;
                                            case INT:
                                                value = std::to_string(func.args[value].int_value);
                                                break;
                                            case FLOAT:
                                                value = std::to_string(func.args[value].float_value);
                                                break;
                                            case CHAR:
                                                value = std::to_string(func.args[value].char_value);
                                                break;
                                            case STRING:
                                                value = func.args[value].string_value;
                                                break;
                                        }
                                    }
                                }

                                std::cout << value << std::endl;
                                break;
                        }
                    }
                    break;
                }
                case VARIABLE:
                {
                    Variable v;
                    functions[current_function.name].variables.insert(std::pair<std::string, Variable>(current_instruction.keyword, v));
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
                    Function_Stack[Function_Stack.size() - 1].code_index = index + 1;
                    Function_Stack_Data new_function;
                    new_function.name = current_instruction.keyword;
                    new_function.code_index = 0;
                    Function_Stack.push_back(new_function);

                    for (int arg = 0; arg < functions[current_instruction.keyword].args_order.size(); arg++)
                    {
                        Variable current_arg = functions[new_function.name].args[functions[current_instruction.keyword].args_order[arg]];
                        Token value = functions[current_function.name].code[index + arg + 1];
                        
                        if (value.type == REFERENCE)
                        {
                            Variable v = functions[current_function.name].variables[value.keyword];

                            if (v.type == STRING)
                                value.keyword = v.string_value;
                            else if (v.type == INT)
                                value.keyword = std::to_string(v.int_value);
                            else if (v.type == FLOAT)
                                value.keyword = std::to_string(v.float_value);
                            else if (v.type == CHAR)
                                value.keyword = std::to_string(v.char_value);
                        }
                        
                        if (current_arg.type == INT) functions[new_function.name].args[functions[current_instruction.keyword].args_order[arg]].int_value = std::stoi(value.keyword);
                        else if (current_arg.type == FLOAT) functions[new_function.name].args[functions[current_instruction.keyword].args_order[arg]].float_value = std::stof(value.keyword);
                        else if (current_arg.type == CHAR) functions[new_function.name].args[functions[current_instruction.keyword].args_order[arg]].char_value = value.keyword[0];
                        else if (current_arg.type == STRING) functions[new_function.name].args[functions[current_instruction.keyword].args_order[arg]].string_value = value.keyword;
                    }

                    index = functions[current_function.name].code.size();
                    break;
                }
                case END:
                    Function_Stack.pop_back();
                    index = functions[current_function.name].code.size();
                    break;
                default:
                    break;
            }
        }
    }
    return 0;
}