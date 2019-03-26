#include "parallel_encoder.h"
#ifdef _WIN32
#define HAVE_STRUCT_TIMESPEC
#include "pthread.h"
#endif
#include "encoder.h"
#include <iostream>


pthread_mutex_t pthread_mutex;

parallel_encoder::parallel_encoder()
{}

parallel_encoder::~parallel_encoder()
{}

void parallel_encoder::run(std::vector<std::string> file_paths)
{
    auto num_threads = file_paths.size();
    std::vector<pthread_t> threads(num_threads);

    pthread_mutex = PTHREAD_MUTEX_INITIALIZER;

    for(size_t i = 0; i < num_threads; i++)
    {
        auto result = pthread_create(&threads[i], nullptr, parallel_encoder::encode, static_cast<void*>(&file_paths[i]));
        if(result != 0)
        {
            std::cout << "Couldn't create thread for " << file_paths[i] << std::endl;
        }
    }

    for(size_t i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }
}

void* parallel_encoder::encode(void *file_path) {
    auto wav_file_path = *reinterpret_cast<std::string*>(file_path);

    encoder encoder;
    auto init_result = encoder.init(wav_file_path);

    if(init_result == encoder_status::invalid_format)
    {
        return nullptr;
    }

    if(init_result != encoder_status::status_ok)
    {
        std::cout << "Couldn't initialize encoder for " << wav_file_path << std::endl;
        return nullptr;
    }

    pthread_mutex_lock(&pthread_mutex);
    std::cout << "Encoding " << wav_file_path << std::endl;
    pthread_mutex_unlock(&pthread_mutex);

    if(encoder.encode() != encoder_status::status_ok)
    {
        std::cout << "Couldn't encode" << wav_file_path << std::endl;
    }

    return nullptr;
}
