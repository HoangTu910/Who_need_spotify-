#include "../inc/buffer/wns_BufferManagement.hpp"
#include "../inc/modules/wns_Preprocessing.hpp"
#include "../inc/modules/wns_Limiter.hpp"
#include <iostream>

int main()
{
    const std::string inputPath = "audio/overshoot_test.wav";
    const std::string outputPath = "audio/overshoot_test_limited.wav";

    wns_modules::WNS_Preprocessing audioProcessor;
    wns_infrastructure::BufferChunk inChunk;

    if (audioProcessor.readAudioToBuffer(inputPath, inChunk) != WSN_NO_ERROR) {
        std::cerr << "Failed to read audio to buffer: " << inputPath << std::endl;
        return 1;
    }

    // Setup limiter coefficients
    limiterCoeff coeffs;
    coeffs.fThreshold = 0.01f;    // linear amplitude threshold
    coeffs.fAttackTime = 0.002f;   // per-sample style coefficient (as used in your code)
    coeffs.fReleaseTime = 0.005f; // per-sample style coefficient
    coeffs.iDelay = 128;          // lookahead in samples

    wns_modules::WNS_Limiter limiter;
    if (limiter.vSetLimiterCoeffs(coeffs) != WSN_NO_ERROR) {
        std::cerr << "Failed to set limiter coefficients" << std::endl;
        return 2;
    }

    // Prepare output buffer and process (inChunk -> outChunk)
    wns_infrastructure::BufferChunk outChunk = inChunk; // copy meta and allocate
    if (limiter.vProcess(inChunk, outChunk) != WSN_NO_ERROR) {
        std::cerr << "Limiter processing failed" << std::endl;
        return 3;
    }

    if (audioProcessor.writeBufferToAudio(outputPath, outChunk) != WSN_NO_ERROR) {
        std::cerr << "Failed to write output audio: " << outputPath << std::endl;
        return 4;
    }

    std::cout << "Wrote limited output: " << outputPath << std::endl;
    return 0;
}