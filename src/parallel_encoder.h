#pragma once

#include <vector>
#include <string>


class parallel_encoder
{
public:
    parallel_encoder();
    ~parallel_encoder();

public:
    void         run(std::vector<std::string> file_paths);
    static void* encode(void* file_path);
};
