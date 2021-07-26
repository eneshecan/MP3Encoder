#pragma once

#include "lame/lame.h"
#include <string>
#include <string_view>
#include <memory>

namespace mp3_encoder
{
    enum class EncoderStatus
    {
        StatusOk,
        InvalidFormat,
        OpenFileError,
        InitializationFailed,
        EncodeBufferError
    };


    struct wav_header_t
    {
        char          chunkId[4];
        std::uint32_t chunkSize;
        char          format[4];
        char          subchunk1Id[4];
        std::uint32_t subchunk1Size;
        std::uint16_t audioFormat;
        std::uint16_t numChannels;
        std::uint32_t sampleRate;
        std::uint32_t byteRate;
        std::uint16_t blockAlign;
        std::uint16_t bitsPerSample;
        char          subchunk2Id[4];
        std::uint32_t subchunk2Size;
    };


    class Encoder
    {
        public:
            Encoder() noexcept;
            ~Encoder();

        public:
            EncoderStatus init(std::string_view wavFilePath);
            EncoderStatus encode();

        private:
            lame_global_flags*             m_lameFlags;
            std::string                    m_wavFilePath;
            wav_header_t                   m_wavHeader;
            std::shared_ptr<short>         m_wavBuffer;
            std::shared_ptr<unsigned char> m_mp3Buffer;
    };
}
