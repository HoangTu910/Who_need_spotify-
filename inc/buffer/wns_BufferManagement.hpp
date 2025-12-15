#ifndef _WNS_BUFFER_MANAGEMENT_H_
#define _WNS_BUFFER_MANAGEMENT_H_

#include <vector>
#include <memory>
#include <mutex>
#include <algorithm>
#include <cstddef>
#include "../common/wsn_Common.hpp"

namespace wns_infrastructure
{
// Owning audio buffer chunk. Stores interleaved samples in pBuffer with size = frames*channels.
// Use BufferView for non-owning references to external memory.
struct BufferChunk
{
    size_t frames = 0;       // number of frames
    int channels = 1;        // channels per frame
    int sampleRate = 0;      // sample rate in Hz
    std::vector<tFloat> pBuffer; // interleaved samples: [f0c0,f0c1,...,f1c0,...]

    BufferChunk();
    BufferChunk(size_t frames_, int channels_, int sampleRate_);

    // Access raw pointer to interleaved data (owning)
    tFloat* data();
    const tFloat* data() const;

    // Helper: number of samples (frames * channels)
    size_t samples() const;

    // Zero the buffer
    void clear();

    // Copy from external interleaved buffer (size in samples must be <= samples())
    void copyFrom(const tFloat* src, size_t samplesToCopy);

    // Create a lightweight non-owning view pointing to the internal buffer
    struct BufferView makeView() const;
};

// Non-owning view into interleaved audio data. Safe to create from external buffers or from BufferChunk::makeView().
struct BufferView {
    tFloat* data = nullptr;    // pointer to interleaved samples
    size_t frames = 0;         // number of frames
    int channels = 1;          // number of channels
    bool interleaved = true;   // true for interleaved layout

    constexpr BufferView() = default;
    BufferView(tFloat* data_, size_t frames_, int channels_, bool interleaved_ = true);

    // Access sample by (frame, channel)
    inline tFloat& at(size_t frame, int channel);
    inline const tFloat& at(size_t frame, int channel) const;
};

// Simple pooled buffer manager. This is intended to be used from non-realtime threads.
// allocateBuffer() returns a shared_ptr to a BufferChunk; releaseBuffer() returns it to the pool.
class wns_BufferManager
{
public:
    wns_BufferManager();
    ~wns_BufferManager();

    // Allocate or reuse a buffer chunk. Non-realtime caller.
    std::shared_ptr<BufferChunk> allocateBuffer(size_t frames, int channels, int sampleRate);

    // Return buffer to pool for reuse. Non-realtime caller.
    void releaseBuffer(std::shared_ptr<BufferChunk> buffer);

    // Clear pool (non-realtime)
    void clearPool();

private:
    std::vector<std::shared_ptr<BufferChunk>> m_pool;
    std::mutex m_mutex;
};

} // namespace wns_BufferManagement

#endif // _WNS_BUFFER_MANAGEMENT_H_
