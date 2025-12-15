#include "../inc/buffer/wns_BufferManagement.hpp"

namespace wns_infrastructure
{

BufferChunk::BufferChunk() = default;

BufferChunk::BufferChunk(size_t frames_, int channels_, int sampleRate_)
    : frames(frames_), channels(channels_), sampleRate(sampleRate_), pBuffer(frames_ * channels_, static_cast<tFloat>(0)) {}

tFloat* BufferChunk::data() { return pBuffer.empty() ? nullptr : pBuffer.data(); }

const tFloat* BufferChunk::data() const { return pBuffer.empty() ? nullptr : pBuffer.data(); }

size_t BufferChunk::samples() const { return frames * static_cast<size_t>(channels); }

void BufferChunk::clear() { std::fill(pBuffer.begin(), pBuffer.end(), static_cast<tFloat>(0)); }

void BufferChunk::copyFrom(const tFloat* src, size_t samplesToCopy) {
    size_t n = std::min(samplesToCopy, samples());
    if (n && data()) std::copy(src, src + n, pBuffer.data());
}

BufferView BufferChunk::makeView() const {
    return BufferView(const_cast<tFloat*>(data()), frames, channels, true);
}

// BufferView implementations
BufferView::BufferView(tFloat* data_, size_t frames_, int channels_, bool interleaved_)
    : data(data_), frames(frames_), channels(channels_), interleaved(interleaved_) {}

inline tFloat& BufferView::at(size_t frame, int channel) {
    return data[frame * static_cast<size_t>(channels) + static_cast<size_t>(channel)];
}

inline const tFloat& BufferView::at(size_t frame, int channel) const {
    return data[frame * static_cast<size_t>(channels) + static_cast<size_t>(channel)];
}

// wns_BufferManager implementations
wns_BufferManager::wns_BufferManager() = default;
wns_BufferManager::~wns_BufferManager() = default;

std::shared_ptr<BufferChunk> wns_BufferManager::allocateBuffer(size_t frames, int channels, int sampleRate) {
    std::lock_guard<std::mutex> lk(m_mutex);
    for (auto it = m_pool.begin(); it != m_pool.end(); ++it) {
        auto &ptr = *it;
        if (ptr->channels == channels && ptr->frames >= frames) {
            auto found = ptr;
            m_pool.erase(it);
            if (found->frames != frames) {
                found->frames = frames;
                found->pBuffer.assign(frames * static_cast<size_t>(channels), static_cast<tFloat>(0));
            }
            found->sampleRate = sampleRate;
            return found;
        }
    }
    return std::make_shared<BufferChunk>(frames, channels, sampleRate);
}

void wns_BufferManager::releaseBuffer(std::shared_ptr<BufferChunk> buffer) {
    if (!buffer) return;
    std::lock_guard<std::mutex> lk(m_mutex);
    buffer->clear();
    m_pool.push_back(std::move(buffer));
}

void wns_BufferManager::clearPool() {
    std::lock_guard<std::mutex> lk(m_mutex);
    m_pool.clear();
}

} // namespace wns_BufferManagement
