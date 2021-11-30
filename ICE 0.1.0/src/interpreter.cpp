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
        std::string last_token_keyword = "";
        int current_code_index = 0;

        for (Token t : token_list)
        {
            if (t.type != FUNCTION && t.type != VARIABLE && t.type != DATA_TYPE && t.type != LABEL && t.type != null)
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
                        current_function.args[last_token_keyword].type = type;
                    }
                    break;
                }
                case VARIABLE:
                {
                    if (in_function_code = true)
                    {
                        Variable v;
                        current_function.variables.insert(std::pair<std::string, Variable>(t.keyword, v));
                    } else if (in_function_args == true)
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

            last_token_keyword = t.keyword;
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
                std::cout << a.first << std::endl;
            
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
    main.code_index = -1;
    Function_Stack.push_back(main);

    // Loop through Function_Stack

    while (Function_Stack.size() > 0)
    {
        Function_Stack_Data current_function;
        current_function = Function_Stack[Function_Stack.size() - 1];
        std::cout << current_function.name << std::endl;

        // Loop through current function code

        for (int index = current_function.code_index; index < functions[current_function.name].code.size(); ++index)
        {
            Token current_instruction = functions[current_function.name].code[current_function.code_index];

            if (debug == true)  std::cout << current_instruction.keyword << std::endl;

            switch (current_instruction.type)
            {
                case INSTRUCTION:
                {
                    if (current_instruction.keyword == "output")
                    {
                        switch (std::stoi(functions[current_function.name].code[current_function.code_index + 1].keyword))
                        {
                        case 0:
                            std::cout << functions[current_function.name].code[current_function.code_index + 2].keyword << std::endl;
                            break;
                        
                        default:
                            break;
                        }
                    }
                    break;
                }
                case CALL:
                {
                    Function_Stack[Function_Stack.size() - 1].code_index = index;
                    Function_Stack_Data new_function;
                    new_function.name = current_instruction.keyword;
                    new_function.code_index = -1;
                    Function_Stack.push_back(new_function);
                    break;
                }
                case END:
                    Function_Stack.pop_back();
                    index = functions[current_function.name].code.size() + 1;
                    std::cout << "END" << std::endl;
                    break;
                default:
                    break;
            }
        }
    }
    return 0;
}