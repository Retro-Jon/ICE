#include "dependencies.hpp"
#include "..\ice_data.hpp"
#include "file.hpp"

int main(int argc, char *argv[])
{
    std::map<std::string, std::string> source_files;

    std::string file_data;

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
            std::cout << ICE_HELP << std::endl;
        }
        else if (std::strcmp(argv[1], "-v") == 0 || std::strcmp(argv[1], "-version") == 0)
        {
            std::cout << ICE_VERSION << std::endl;
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
    } else {
        std::cout << "NO INPUT" << std::endl;
    }
    return 0;
}