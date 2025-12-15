#include "../inc/kernels/wns_BiquadFilter.hpp"
#include "../inc/kernels/wns_FFT.hpp"
#include "../inc/buffer/wns_BufferManagement.hpp"
#include "../inc/modules/wns_Preprocessing.hpp"
#include <sndfile.h>
#include <iostream>

int main()
{
    const std::string inputPath = "audio/multi_440_880_1760_2000ms.wav";
    const std::string outputPath = "log/multi_440_880_1760_2000ms_lowpass.wav";

    // Allocate BufferChunk and copy data
    wns_infrastructure::wns_BufferManager *bufferManager;
    wns_modules::WNS_Preprocessing *audioProcessor;
    wns_infrastructure::BufferChunk inChunk;

    if (audioProcessor->readAudioToBuffer(inputPath, inChunk) != WSN_NO_ERROR) {
        std::cerr << "Failed to read audio to buffer" << std::endl;
        return 1;
    }
    audioProcessor->writeBufferToTxt("multi_440_880_1760_2000ms", inChunk);

    // Prepare filter
    wns_Kernels::wns_BiquadFilter biquadFilter;
    biquadFilter.vSetFilterParams(1000, 0.707, 0.0, 1.0);
    biquadFilter.eSetFilterType(wns_Kernels::wns_BiquadType::HIGHPASS);

    wns_infrastructure::BufferChunk outChunk;
    outChunk = inChunk;

    if (biquadFilter.vProcess(inChunk, outChunk) != WSN_NO_ERROR) {
        std::cerr << "Filter processing failed" << std::endl;
        return 3;
    }

    audioProcessor->writeBufferToAudio(outputPath, outChunk);
    audioProcessor->writeBufferToTxt("multi_440_880_1760_2000ms_highpass", outChunk);

    return 0;
}