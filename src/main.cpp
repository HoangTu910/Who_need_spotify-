#include "../inc/kernels/wns_BiquadFilter.hpp"
#include "../inc/kernels/wns_FFT.hpp"

int main()
{
    // Example usage of wns_BiquadFilter
    wns_Kernels::wns_BiquadFilter biquadFilter;
    wns_infrastructure::wns_BufferManager bufferManager;
    
    const size_t frame = 512;
    const int channels = 2;
    const int sampleRate = static_cast<int>(wns_Common::SAMPLING_FREQUENCY);

    auto testChunk = bufferManager.allocateBuffer(frame, channels, sampleRate);

    for (size_t i = 0; i < testChunk->samples(); ++i) {
        testChunk->pBuffer[i] = 0.7f;
    }

    biquadFilter.vSetFilterParams(1000.0, 0.707, 0.0, 1.0);
    biquadFilter.eSetFilterType(wns_Kernels::wns_BiquadType::LOWPASS);

    auto lowPassBuffer = bufferManager.allocateBuffer(frame, channels, sampleRate);
    biquadFilter.vProcess(*testChunk, *lowPassBuffer);

    for(int i = 0; i < lowPassBuffer->samples(); ++i) {
        std::cout << lowPassBuffer->pBuffer[i] << std::endl;
    }

    return 0;
}