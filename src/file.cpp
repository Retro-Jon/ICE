#include "dependencies.hpp"

std::string load(std::string path)
{
    std::string data;
    std::fstream file;
    file.open(path);
    
    if (file)
    {
        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                data += line;
            }
            file.close();
        }
        return data;
    } else {
        std::cout << "Unable to open file." << std::endl;
        return "NOT FOUND";
    }
}

int save_string(std::string path, std::string data)
{
    std::fstream file;
    file.open(path);
    
    if (file)
    {
        if (file.is_open())
        {
            file << data;
            file.close();
        }
        return 0;
    } else {
        std::cout << "Unable to open file." << std::endl;
        return 1;
    }
}