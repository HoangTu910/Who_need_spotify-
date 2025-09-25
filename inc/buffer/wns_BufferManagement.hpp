#ifndef _WNS_BUFFER_MANAGEMENT_H_
#define _WNS_BUFFER_MANAGEMENT_H_

#include <vector>
#include "../common/wsn_Common.hpp"

namespace wns_BufferManagement
{
struct BufferChunk
{
    size_t size;
    size_t refCount;
    int channel;
    int sampleRate;
    std::vector<tFloat> pBuffer;

    BufferChunk(size_t sz, int ch, int sr)
        : size(sz), refCount(1), channel(ch), sampleRate(sr), pBuffer(sz * ch, 0.0f) {}
};

class wns_BufferManager
{
public:
    wns_BufferManager();
    ~wns_BufferManager();

    void allocateBuffer(size_t size);
    void deallocateBuffer();

private:
    std::vector<char> m_buffer;
};
}

#endif
