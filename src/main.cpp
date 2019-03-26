#ifdef _WIN32
#include "dirent_win.h"
#else
#include <dirent.h>
#endif

#include <iostream>
#include <sstream>
#include "parallel_encoder.h"


//TODO Add --help section
//TODO Fix cracking sound problem at the beginning


int main(int argc, char* args[])
{
    struct dirent *p_dirent = nullptr;
    DIR *p_dir;

    if (argc < 2) {
        std::cout << "Usage: MP3Encoder <directory name>" << std::endl;
        return 1;
    }

    std::string dir_name(args[1]);

    p_dir = opendir(dir_name.c_str());
    if (!p_dir) {
        std::cout << "Cannot open directory " << args[1] << std::endl;
        return 1;
    }

    std::vector<std::string> paths;

    while ((p_dirent = readdir(p_dir)) != nullptr)
    {
        std::string name(p_dirent->d_name);
        if((name == ".") || (name == ".."))
        {
            continue;
        }

        std::stringstream path;
        path << dir_name << "/" << name;

        paths.push_back(path.str());
    }
    closedir (p_dir);


    parallel_encoder batch_encoder;
    batch_encoder.run(paths);

    return 0;
}