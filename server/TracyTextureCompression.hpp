#ifndef __TRACY__TEXTURECOMPRESSION_HPP__
#define __TRACY__TEXTURECOMPRESSION_HPP__

#include <atomic>

#include <stdint.h>
#include <stdlib.h>

#include "TracySlab.hpp"

struct ZSTD_CCtx_s;
struct ZSTD_DCtx_s;

namespace tracy
{

struct FrameImage;

class TextureCompression
{
public:
    TextureCompression();
    ~TextureCompression();

    uint32_t Pack( struct ZSTD_CCtx_s* ctx, char*& buf, size_t& bufsz, const char* image, uint32_t inBytes );

    template<size_t Size>
    const char* Pack( const char* image, uint32_t inBytes, uint32_t& csz, Slab<Size>& slab )
    {
        const auto outsz = Pack( m_cctx, m_buf, m_bufSize, image, inBytes );
        auto ptr = (char*)slab.AllocBig( outsz );
        memcpy( ptr, m_buf, outsz );
        csz = outsz;
        return ptr;
    }

    const char* Unpack( const FrameImage& image );

    uint64_t GetInputBytesCount() const { return m_inputBytes.load( std::memory_order_relaxed ); }
    uint64_t GetOutputBytesCount() const { return m_outputBytes.load( std::memory_order_relaxed ); }

private:
    char* m_buf;
    size_t m_bufSize;
    struct ZSTD_CCtx_s* m_cctx;
    struct ZSTD_DCtx_s* m_dctx;

    std::atomic<uint64_t> m_inputBytes { 0 };
    std::atomic<uint64_t> m_outputBytes { 0 };
};

}

#endif
