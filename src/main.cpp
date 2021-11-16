#include "dependencies.hpp"
#include "..\ice_data.hpp"
#include "ice_functions.hpp"
#include "file.hpp"

int main(int argc, char *argv[])
{
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

            for (Token t : tokens)
            {
                if (t.type == INSTRUCTION) std::cout << "INSTRUCTION: ";
                else if (t.type == LABEL) std::cout << "LABEL: ";
                else if (t.type == FUNCTION) std::cout << "FUNCTION: ";
                else if (t.type == METHOD) std::cout << "METHOD: ";
                else if (t.type == VARIABLE) std::cout << "VARIABLE: ";
                else if (t.type == DATA_TYPE) std::cout << "DATA_TYPE: ";
                else if (t.type == OPPERATOR) std::cout << "OPPERATOR: ";
                
                std::cout << t.keyword << std::endl;
            }
        }   
    } else {
        std::cout << "NO INPUT" << std::endl;
    }
    return 0;
}