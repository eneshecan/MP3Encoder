#include "Encoder.hpp"


namespace mp3_encoder
{
    namespace
    {
        constexpr auto const CHUNK_SIZE = std::size_t{4096};
    }

    // TODO Measure performance and improve

    Encoder::Encoder() noexcept
        : m_lameFlags{nullptr}
        , m_wavFilePath{}
        , m_wavHeader{}
        , m_wavBuffer{}
        , m_mp3Buffer{}
    {}

    Encoder::~Encoder()
    {
        if (m_lameFlags)
        {
            lame_close(m_lameFlags);
        }
    }

    EncoderStatus Encoder::init(std::string_view wavFilePath)
    {
        if (wavFilePath.substr(wavFilePath.find_last_of('.') + 1U) != "wav")
        {
            return EncoderStatus::InvalidFormat;
        }

        m_wavFilePath = wavFilePath;

        auto const wavFile = fopen(m_wavFilePath.c_str(), "rb");
        if(!wavFile)
        {
            return EncoderStatus::OpenFileError;
        }

        fread(&m_wavHeader, sizeof(wav_header_t), 1, wavFile);

        fclose(wavFile);

        m_lameFlags = lame_init();

        if (!m_lameFlags)
        {
            return EncoderStatus::InitializationFailed;
        }

        lame_set_num_channels(m_lameFlags, m_wavHeader.numChannels);
        lame_set_in_samplerate(m_lameFlags, static_cast<int>(m_wavHeader.sampleRate));
        lame_set_out_samplerate(m_lameFlags, static_cast<int>(m_wavHeader.sampleRate));
        lame_set_quality(m_lameFlags, 5);

        if (lame_init_params(m_lameFlags) == -1)
        {
            return EncoderStatus::InitializationFailed;
        }

        m_wavBuffer.reset(new short[CHUNK_SIZE * m_wavHeader.numChannels]);
        m_mp3Buffer.reset(new unsigned char[CHUNK_SIZE * m_wavHeader.numChannels]);

        return EncoderStatus::StatusOk;
    }

    EncoderStatus Encoder::encode()
    {
        auto const wavFile = fopen(m_wavFilePath.c_str(), "rb");
        if(!wavFile)
        {
            return EncoderStatus::OpenFileError;
        }

        std::string mp3_file_path = m_wavFilePath.replace(m_wavFilePath.find_last_of('.') + 1, 3, "mp3");
        FILE* mp3File = fopen(mp3_file_path.c_str(), "wb");
        if(!mp3File)
        {
            return EncoderStatus::OpenFileError;
        }

        fread(&m_wavHeader, sizeof(wav_header_t), 1, wavFile);

        auto bytesRead    = std::size_t{};
        auto bytesWritten = std::size_t{};

        do
        {
            bytesRead = fread(m_wavBuffer.get(), m_wavHeader.numChannels * sizeof(short), CHUNK_SIZE, wavFile);

            if (bytesRead != 0)
            {
                if (m_wavHeader.numChannels == 1)
                {
                    bytesWritten = lame_encode_buffer(m_lameFlags, m_wavBuffer.get(), nullptr, static_cast<int>(bytesRead), m_mp3Buffer.get(), CHUNK_SIZE);
                }
                else
                {
                    bytesWritten = lame_encode_buffer_interleaved(m_lameFlags, m_wavBuffer.get(), static_cast<int>(bytesRead), m_mp3Buffer.get(), CHUNK_SIZE);
                }
            }
            else
            {
                bytesWritten = lame_encode_flush(m_lameFlags, m_mp3Buffer.get(), CHUNK_SIZE);
            }

            if (bytesWritten < 0)
            {
                fclose(wavFile);
                fclose(mp3File);
                return EncoderStatus::EncodeBufferError;
            }

            fwrite(m_mp3Buffer.get(), bytesWritten, 1, mp3File);

        } while (bytesRead != 0);

        fclose(wavFile);
        fclose(mp3File);

        return EncoderStatus::StatusOk;
    }
}
