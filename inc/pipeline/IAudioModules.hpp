#ifndef WNS_IAUDIOMODULES_HPP
#define WNS_IAUDIOMODULES_HPP

#include "../common/wsn_ErrorCodes.hpp"
#include "../buffer/wns_BufferManagement.hpp"
#include <cstddef>

namespace wns_modules {

// Use buffer management's BufferChunk for module processing. This type describes an audio
// buffer (size, channels, sampleRate) and contains an internal float vector `pBuffer`.
// Implementations MUST NOT take ownership of external buffers unless explicitly documented.
// Richer interface for audio processing modules.
// - prepare(...) is intended to be called on a non-realtime thread and may allocate.
// - vProcess(...) is called on the realtime/audio thread and MUST NOT allocate, lock, or throw.
// - All realtime-facing methods should return wsn_eF to indicate errors deterministically.
class IAudioModule {
public:
    virtual ~IAudioModule() = default;

    // Prepare the module (called before audio processing starts). Can allocate and compute heavy data.
    virtual wsn_eF prepare(double sampleRate, size_t maxFrames, int channels) { (void)sampleRate; (void)maxFrames; (void)channels; return WSN_NO_ERROR; }

    // Reset internal state (e.g., on transport stop). Non-realtime.
    virtual void reset() { }

    // Process audio buffer on the realtime thread. `buffer` references a BufferChunk which
    // contains metadata and a float vector `pBuffer`. Implementations must treat this
    // call as realtime-critical and must not perform allocations or locking.
    virtual wsn_eF vProcess(const wns_infrastructure::BufferChunk& bufferIn, wns_infrastructure::BufferChunk& bufferOut) = 0;

    // Release resources allocated in prepare(). Non-realtime.
    virtual void release() { }

    // Optional friendly name for debugging.
    virtual const char* sName() const { return "IAudioModule"; }

    // Backwards-compatible wrapper for older modules; calls the new vProcess by creating a simple view.
    // Deprecated: prefer overriding the AudioBufferView-based vProcess instead.
    [[deprecated("Use vProcess(const wns_BufferManagement::BufferChunk&) instead")]]
    virtual wsn_eF vProcess(float* buffer, int numSamples) {
        // Deprecated wrapper: create a temporary BufferChunk and copy the data into it.
        // This is non-realtime and intended only for backwards compatibility.
        wns_infrastructure::BufferChunk tmp(static_cast<size_t>(numSamples), 1, static_cast<int>(wns_Common::SAMPLING_FREQUENCY));
        size_t copyCount = std::min(tmp.pBuffer.size(), static_cast<size_t>(numSamples));
        for (size_t i = 0; i < copyCount; ++i) tmp.pBuffer[i] = buffer[i];
        return vProcess(tmp, tmp);
    }
};

} // namespace wns_modules

#endif // WNS_IAUDIOMODULES_HPP
