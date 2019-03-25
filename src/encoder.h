#pragma once

#include "lame/lame.h"
#include <string>
#include <memory>

//TODO Create enum class to represent error codes from init and encode functions


struct wav_header_t
{
    char           chunk_id_[4];
    uint32_t       chunk_size_;
    char           format_[4];
    char           subchunk1_id_[4];
    uint32_t       subchunk1_size_;
    uint16_t       audio_format_;
    uint16_t       num_channels_;
    uint32_t       sample_rate_;
    uint32_t       byte_rate_;
    uint16_t       block_align_;
    uint16_t       bits_per_sample_;
};


class encoder
{
public:
    encoder();
    ~encoder();

public:
    int init(std::string wav_file_path);
    bool encode();

private:
    lame_global_flags*             lame_flags_;
    std::string                    wav_file_path_;
    wav_header_t                   wav_header_;
    std::shared_ptr<short>         wav_buffer_;
    std::shared_ptr<unsigned char> mp3_buffer_;
    const size_t                   chunk_size_;
};
