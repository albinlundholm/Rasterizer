#include "ObjLoader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

Mesh load_obj(char *path){
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error(std::string("could not open file: ") + path);
    }

    int v_count = 0;
    int f_count = 0;

    std::string line;
    while (std::getline(file, line))
    {
        std::string token;
        std::istringstream ss(line);
        ss >> token;
        if (token == "v")
        {
            v_count++;
        }
        if (token == "f")
        {
            f_count++;
        }
    }
    

    file.close();
    
}