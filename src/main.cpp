#include <dirent.h>
#include <iostream>
#include <sstream>
#include "ParallelEncoder.hpp"


int main(int argc, char** args)
{
    if (argc < 2)
    {
        std::cerr << "Usage: MP3Encoder <directory name>" << std::endl;
        return 1;
    }

    auto const dirName = args[1];
    auto const dir = opendir(dirName);
    if (!dir)
    {
        std::cerr << "Failed to open directory " << dirName << std::endl;
        return 1;
    }

    auto paths = std::vector<std::string>{};

    for (dirent* dirent = nullptr; (dirent = readdir(dir)) != nullptr;)
    {
        auto const name = std::string{dirent->d_name};
        if ((name == ".") || (name == ".."))
        {
            continue;
        }

        auto path = std::stringstream{};
        path << dirName << "/" << name;

        paths.push_back(path.str());
    }

    closedir(dir);

    mp3_encoder::parallel_encoder::run(paths);

    return 0;
}