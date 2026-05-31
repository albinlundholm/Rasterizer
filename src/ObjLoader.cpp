#include "ObjLoader.h"
#include "Vec3.h"

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

    // Do a counting pass to get number of vertices and faces
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
    
    file.clear();
    file.seekg(0);

    Vec3 *vertices = (Vec3 *)malloc(v_count * sizeof(Vec3));
    Face *faces = (Face *)malloc(f_count * sizeof(Face));

    // Read file vertices and faces into memory
    float x;
    float y;
    float z;

    int v_read = 0;
    int f_read = 0;

    while (std::getline(file, line))
    {
        std::string token;
        std::istringstream ss(line);
        ss >> token;
        if (token == "v")
        {
            ss >> x;
            ss >> y;
            ss >> z;

            vertices[v_read] = Vec3 {x, y, z};
            v_read++;
        }
        if (token == "f")
        {
            int v0, v1, v2;
            ss >> token;
            {
            std::istringstream entry(token);
            std::string part;
            std::getline(entry, part, '/');
            v0 = std::stoi(part) - 1;
            }

            ss >> token;
            {
            std::istringstream entry(token);
            std::string part;
            std::getline(entry, part, '/');
            v1 = std::stoi(part) - 1;
            }

            ss >> token;
            {
            std::istringstream entry(token);
            std::string part;
            std::getline(entry, part, '/');
            v2 = std::stoi(part) - 1;
            }

            faces[f_read] = {v0, v1, v2};
            f_read++;
        }
    }

    Mesh obj = {
        vertices,
        faces,
        v_count,
        f_count,       
    };
    
    file.close();

    return obj;
}