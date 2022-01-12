#include "dependencies.hpp"

std::string load_string(std::string path)
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
                data += line + '\n';
            }
            file.close();
        }
    } else {
        std::cout << "Unable to open file: " << path << std::endl;
        data = "NOT FOUND";
    }

    return data;
}

int save_string(std::string path, std::string data)
{
    std::fstream file;
    file.open(path, std::fstream::out);
    
    if (file)
    {
        if (file.is_open())
        {
            file << data;
        }
        file.close();
    } else {
        file.close();
        std::cout << "Cannot open file: " << path << std::endl;
        return 1;
    }
    return 0;
}