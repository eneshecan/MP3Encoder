#include "encoder.h"


encoder::encoder() :
    lame_flags_(nullptr),
    wav_header_({}),
    chunk_size_(4096)
{}

encoder::~encoder()
{
    if(lame_flags_)
    {
        lame_close(lame_flags_);
    }
}

encoder_status encoder::init(std::string wav_file_path)
{
    if(wav_file_path.substr(wav_file_path.find_last_of('.') + 1) != "wav")
    {
        return encoder_status::invalid_format;
    }

    wav_file_path_ = wav_file_path;

    FILE* wav_file = fopen(wav_file_path_.c_str(), "rb");
    if(!wav_file)
    {
        return encoder_status::open_file_error;
    }

    fread(&wav_header_, sizeof(wav_header_t), 1, wav_file);

    fclose(wav_file);

    lame_flags_ = lame_init();

    if(!lame_flags_)
    {
        return encoder_status::initialization_failed;
    }

    lame_set_num_channels(lame_flags_, wav_header_.num_channels_);
    lame_set_in_samplerate(lame_flags_, wav_header_.sample_rate_);
    lame_set_out_samplerate(lame_flags_, wav_header_.sample_rate_);
    lame_set_quality(lame_flags_, 5);

    if(lame_init_params(lame_flags_) == -1)
    {
        return encoder_status::initialization_failed;
    }

    wav_buffer_.reset(new short[chunk_size_ * wav_header_.num_channels_]);
    mp3_buffer_.reset(new unsigned char[chunk_size_ * wav_header_.num_channels_]);

    return encoder_status::status_ok;
}

encoder_status encoder::encode()
{
    FILE* wav_file = fopen(wav_file_path_.c_str(), "rb");
    if(!wav_file)
    {
        return encoder_status::open_file_error;
    }

    std::string mp3_file_path = wav_file_path_.replace(wav_file_path_.find_last_of(".") + 1, 3, "mp3");
    FILE* mp3_file = fopen(mp3_file_path.c_str(), "wb");
    if(!mp3_file)
    {
        return encoder_status::open_file_error;
    }

    fread(&wav_header_, sizeof(wav_header_t), 1, wav_file);

    size_t bytes_read = 0;
    size_t bytes_wrote = 0;

    do
    {
        bytes_read = fread(wav_buffer_.get(), wav_header_.num_channels_ * sizeof(short), chunk_size_, wav_file);

        if(bytes_read != 0)
        {
            if(wav_header_.num_channels_ == 1)
            {
                bytes_wrote = lame_encode_buffer(lame_flags_, wav_buffer_.get(), nullptr, bytes_read, mp3_buffer_.get(), chunk_size_);
            }
            else
            {
                bytes_wrote = lame_encode_buffer_interleaved(lame_flags_, wav_buffer_.get(), bytes_read, mp3_buffer_.get(), chunk_size_);
            }
        }
        else
        {
            bytes_wrote = lame_encode_flush(lame_flags_, mp3_buffer_.get(), chunk_size_);
        }

        if(bytes_wrote < 0)
        {
            fclose(wav_file);
            fclose(mp3_file);
            return encoder_status::encode_buffer_error;
        }

        fwrite(mp3_buffer_.get(), bytes_wrote, 1, mp3_file);

    } while(bytes_read != 0);

    fclose(wav_file);
    fclose(mp3_file);

    return encoder_status::status_ok;
}
