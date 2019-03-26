#ifdef _WIN32
#include "dirent.h"
#else
#include <dirent.h>
#endif
#include <iostream>
#include <sstream>
#include "parallel_encoder.h"


int main(int argc, char* args[])
{
    if (argc < 2) {
        std::cout << "Usage: MP3Encoder <directory name>" << std::endl;
        return 1;
    }

    std::string dir_name(args[1]);

    struct dirent *dirent = nullptr;
    DIR *dir = nullptr;

    dir = opendir(dir_name.c_str());
    if (!dir) {
        std::cout << "Couldn't open directory " << args[1] << std::endl;
        return 1;
    }

    std::vector<std::string> paths;

    while ((dirent = readdir(dir)) != nullptr)
    {
        std::string name(dirent->d_name);
        if((name == ".") || (name == ".."))
        {
            continue;
        }

        std::stringstream path;
        path << dir_name << "/" << name;

        paths.push_back(path.str());
    }
    closedir(dir);


    parallel_encoder parallel_encoder;
    parallel_encoder.run(paths);

    return 0;
}