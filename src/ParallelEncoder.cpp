#include "ParallelEncoder.hpp"
#include "Encoder.hpp"
#include <pthread.h>
#include <iostream>


namespace mp3_encoder::parallel_encoder
{
    // TODO Fix overwriting files segfault

    namespace
    {
        auto mutex = pthread_mutex_t{};

        void* encode(void *filePath)
        {
            auto wavFilePath = *reinterpret_cast<std::string*>(filePath);

            auto encoder = Encoder{};
            auto const result = encoder.init(wavFilePath);

            if (result == EncoderStatus::InvalidFormat)
            {
                std::cerr << "Invalid format: " << wavFilePath << std::endl;
                return nullptr;
            }

            if (result != EncoderStatus::StatusOk)
            {
                std::cerr << "Failed to initialize Encoder for " << wavFilePath << std::endl;
                return nullptr;
            }

            pthread_mutex_lock(&mutex);
            std::cerr << "Encoding " << wavFilePath << std::endl;
            pthread_mutex_unlock(&mutex);

            if (encoder.encode() != EncoderStatus::StatusOk)
            {
                std::cerr << "Failed to encode" << wavFilePath << std::endl;
            }

            return nullptr;
        }
    }

    void run(std::vector<std::string>& filePaths)
    {
        mutex = PTHREAD_MUTEX_INITIALIZER;

        auto const numThreads = filePaths.size();
        auto threads = std::vector<pthread_t>{numThreads};

        for (auto i = std::size_t{0}; i < numThreads; ++i)
        {
            auto const result = pthread_create(&threads[i], nullptr, encode, static_cast<void*>(&filePaths[i]));
            if (result != 0)
            {
                std::cout << "Failed to create thread for " << filePaths[i] << std::endl;
            }
        }

        for (auto i = std::size_t{0}; i < numThreads; ++i)
        {
            pthread_join(threads[i], nullptr);
        }
    }
}
