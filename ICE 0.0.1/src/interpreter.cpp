#include "dependencies.hpp"

int run(std::vector<Token> token_list, bool debug)
{
    int length = token_list.size();
    Token tokens[length];
    for (int i = 0; i < length; i++)
        tokens[i] = token_list[i];
    
    std::map<std::string, Function> functions;
    
    /* Scan for functions */
    {
        Function current_function;
        std::string current_function_name;
        bool in_code;

        for (int t = 0; t < length; t++)
        {
            Token current;
            current = tokens[t];

            if (current.type == START)
                in_code = true;
            else if (current.type == DATA_TYPE && current_function.code.size() == 0 && current_function.args.size() == 0)
                current_function.return_type = current.keyword;
            
            if (in_code == true)
                current_function.code.push_back(current);
            else if (in_code == false)
            {
                Variable arg;
                current_function.args.insert(std::pair<std::string, Variable>(current.keyword, arg));
            }

            if (current.type == FUNCTION)
                current_function_name = current.keyword;
            else if (current.type == END)
            {
                functions.insert(std::pair<std::string, Function>(current_function_name, current_function));
                Function new_function;
                current_function = new_function;
                current_function_name = "";
                in_code = false;
            }
        }
    }

    /* List Functions */
    
    if (debug == true)
    {
        for (std::pair<std::string, Function> f : functions)
        {
            std::cout << f.first << std::endl;
            for (Token t : f.second.code)
                std::cout << t.keyword << std::endl;
            std::cout << "\n++++++\n" << std::endl;
        }
        
        std::cout << "\n----------\n";
    }
    
    /* RUN */

    struct previous_function
    {
        std::string name;
        int code_index;
    };

    std::vector<previous_function> Function_Stack;
    previous_function main;
    main.name = "main";
    main.code_index = 0;
    Function_Stack.push_back(main);

    while (Function_Stack.size() > 0)
    {
        int f = Function_Stack.size() - 1;
        int current_function_index = Function_Stack.size() - 1;
        Function current_function = functions[Function_Stack[current_function_index].name];

        if (debug == true) std::cout << "CURRENT_FUNCTION: " << Function_Stack[current_function_index].name << " : " << current_function_index << std::endl;

        previous_function last_func_data;
        last_func_data.name = Function_Stack[current_function_index].name;
        last_func_data.code_index = Function_Stack[current_function_index].code_index;

        for (int code_index = last_func_data.code_index; code_index < current_function.code.size(); code_index++)
        {
            Token current_code = current_function.code[code_index];
            if (debug == true) std::cout << code_index << " : " << current_code.keyword << std::endl;

            switch (current_code.type)
            {
                case VARIABLE:
                {
                    Variable new_variable;
                    current_function.variables.insert(std::pair<std::string, Variable>(current_code.keyword, new_variable));
                    break;
                }
                case DATA_TYPE:
                {
                    int t = 0;

                    if (current_code.keyword == "void")
                        t = 0;
                    else if (current_code.keyword == "int")
                        t = 1;
                    else if (current_code.keyword == "float")
                        t = 2;
                    else if (current_code.keyword == "char")
                        t = 3;
                    else if (current_code.keyword == "string")
                        t = 4;
                    
                    current_function.variables[current_function.variables.end()->first].type = t;
                    break;
                }
                case CALL:
                {
                    last_func_data.code_index = code_index + 1;
                    Function_Stack[f].code_index = last_func_data.code_index;

                    previous_function new_func;
                    new_func.name = current_code.keyword;
                    new_func.code_index = 0;
                    int arg_counter = 0;
                    std::vector<std::string> arg_names;

                    for (std::pair<std::string, Variable> arg : functions[new_func.name].args)
                        arg_names.push_back(arg.first);

                    Function_Stack.push_back(new_func);
                    f++;
                    
                    code_index = current_function.code.size();
                    break;
                }
                case END:
                    Function_Stack.pop_back();
                    code_index = current_function.code.size();
                    break;
                case INSTRUCTION:
                {
                    if (current_code.keyword == "output")
                    {
                        switch (std::stoi(current_function.code[code_index + 1].keyword))
                        {
                            case 0:
                            {
                                Token out;

                                if (current_function.variables.count(current_function.code[code_index + 2].keyword) != 0)
                                    out.keyword = current_function.variables[current_function.code[code_index + 2].keyword].string_value;

                                std::cout << out.keyword << std::endl;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    if (debug == true) std::cout << "---------" << std::endl;
    return 0;
}