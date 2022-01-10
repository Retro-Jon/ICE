#include "dependencies.hpp"
#include "..\ice_data.hpp"
#include "ice_functions.hpp"
#include "file.hpp"

bool dev_debug = true;

int main(int argc, char *argv[])
{
    int exit_code = 0;
    std::map<std::string, std::string> source_files;

    if (argc >= 3)
    {
        for (int i = 2; i < argc; i++)
        {
            source_files.insert(std::pair<std::string, std::string>(argv[i], load_string(argv[i])));
        }
    }

    if (argc == 2)
    {
        if (std::strcmp(argv[1], "-h") == 0 || std::strcmp(argv[1], "-help") == 0)
        {
            std::cout << _HELP << std::endl;
        }
        else if (std::strcmp(argv[1], "-v") == 0 || std::strcmp(argv[1], "-version") == 0)
        {
            std::cout << _VERSION << std::endl;
        }
        else if (std::strcmp(argv[1], "-f") == 0 || std::strcmp(argv[1], "-features") == 0)
        {
            std::cout << _FEATURES << std::endl;
        }
    }
    else if (argc >= 3)
    {
        if (std::strcmp(argv[1], "-l") == 0 || std::strcmp(argv[1], "-list") == 0)
        {
            if (source_files.size() > 0)
            {
                for (std::pair<std::string, std::string> p : source_files)
                {
                    std::cout << "--" << p.first << "--\n" << p.second << std::endl;
                }
            } else {
                std::cout << "NO FILE INPUT" << std::endl;
            }
        }
        else if (std::strcmp(argv[1], "-r") == 0 || std::strcmp(argv[1], "-run") == 0)
        {
            std::vector<Token> tokens;
            tokens = Lex(source_files);

            if (dev_debug == true)
            {
                for (Token t : tokens)
                {
                    switch (t.type)
                    {
                        case INSTRUCTION:
                            std::cout << "INSTRUCTION: ";
                            break;
                        case LABEL:
                            std::cout << "LABEL: ";
                            break;
                        case FUNCTION:
                            std::cout << "FUNCTION: ";
                            break;
                        case VARIABLE:
                            std::cout << "VARIABLE: ";
                            break;
                        case DATA_TYPE:
                            std::cout << "DATA_TYPE: ";
                            break;
                        case OPPERATOR:
                            std::cout << "OPPERATOR: ";
                            break;
                        case null:
                            std::cout << "null: ";
                            break;
                        case VALUE:
                            std::cout << "VALUE: ";
                            break;
                        case CALL:
                            std::cout << "CALL: ";
                            break;
                        case REFERENCE:
                            std::cout << "REFERENCE: ";
                            break;
                        case END:
                            std::cout << "END: ";
                            break;
                        case START:
                            std::cout << "START: ";
                            break;
                    }
                    std::cout << t.keyword << std::endl;
                }
            }

            if (dev_debug == true) std::cout << "\n-----------\n";

            exit_code = run(tokens, dev_debug);

            std::vector<Token> tmp;
            tokens.swap(tmp);
        }
    } else {
        std::cout << "NO INPUT" << std::endl;
    }

    switch (exit_code)
    {
        case 1:
            std::cout << "EXIT ON ERROR\n";
            break;
    }

    return exit_code;
}