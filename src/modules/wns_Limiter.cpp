#include "../inc/modules/wns_Limiter.hpp"
#include <cmath>
#include <algorithm>
#include "../inc/modules/wns_Preprocessing.hpp"

namespace wns_modules {

wsn_eF WNS_Limiter::vSetLimiterCoeffs(limiterCoeff &coeff) {
    sCoeff = coeff;
    sCoeff.isSet = true;
    // sensible defaults
    if (nDownSampleFactor <= 0) nDownSampleFactor = 4;
    if (sCoeff.iDelay <= 0) sCoeff.iDelay = 5;

    // reinitialize runtime state next time
    bStateInitialized = false;
    return WSN_NO_ERROR;
}

// Helper: initialize per-channel buffers/state
// Member implementation: compute per-channel downsampled gain buffer
wsn_eF WNS_Limiter::vGetGainBuffer(const wns_infrastructure::BufferChunk& bufferIn, wns_infrastructure::BufferChunk& /*bufferOut*/)
{
    if (!sCoeff.isSet) return WSN_UNSET_LIMITER_PARAMETERS;
    if (bufferIn.samples() == 0) return WSN_INVALID_ARGUMENT;

    int channels = bufferIn.channels;
    size_t frames = bufferIn.frames;
    int down = std::max(1, nDownSampleFactor);
    int D = std::max(0, sCoeff.iDelay); // lookahead in samples

    // initialize delay buffers if lookahead requested
    if (D > 0 && (!bStateInitialized || (int)delayBuffers.size() != channels)) {
        delayBuffers.assign(channels, std::vector<float>(D, 0.0f));
        writePos.assign(channels, 0);
        // keep bStateInitialized true as we already track other state
        bStateInitialized = true;
    }
    size_t blocks = (frames + down - 1) / down;

    // init state if needed
    if (!bStateInitialized || (int)xpeakPerChannel.size() != channels) {
        pGainBuffer.assign(channels, std::vector<float>());
        xpeakPerChannel.assign(channels, 0.0f);
        gPerChannel.assign(channels, 1.0f);
        bStateInitialized = true;
    }

    for (int ch = 0; ch < channels; ++ch) {
        pGainBuffer[ch].clear();
        pGainBuffer[ch].reserve(blocks);
    }

    const float *in = bufferIn.data();
    const float EPS = 1e-12f;

    for (int ch = 0; ch < channels; ++ch) {
        float xpeak = xpeakPerChannel[ch];
        float gstate = gPerChannel[ch];

        for (size_t b = 0; b < blocks; ++b) {
            size_t frameIdx = b * down;
            if (frameIdx >= frames) frameIdx = frames - 1;
            size_t idx = frameIdx * channels + ch;
            float sample = std::abs(in[idx]);

            float coeff = (sample > xpeak) ? sCoeff.fAttackTime : sCoeff.fReleaseTime;
            xpeak = (1.0f - coeff) * xpeak + coeff * sample;

            float f = 1.0f;
            if (xpeak > EPS) f = std::min(1.0f, sCoeff.fThreshold / xpeak);

            coeff = (f < gstate) ? sCoeff.fAttackTime : sCoeff.fReleaseTime;
            gstate = (1.0f - coeff) * gstate + coeff * f;

            pGainBuffer[ch].push_back(gstate);
        }

        xpeakPerChannel[ch] = xpeak;
        gPerChannel[ch] = gstate;
    }

    // Optionally write the computed per-block gains to log/ using the preprocessing helper.
    // We create a small interleaved BufferChunk of size (blocks x channels) where each
    // frame contains one gain value per channel for that block. This is non-realtime
    // safe if called from a non-realtime context; failures are non-fatal
    if (blocks > 0) {
        try {
            wns_infrastructure::BufferChunk gainsChunk(blocks, channels, bufferIn.sampleRate);
            for (size_t b = 0; b < blocks; ++b) {
                for (int ch = 0; ch < channels; ++ch) {
                    float g = 1.0f;
                    if (b < pGainBuffer[ch].size()) g = pGainBuffer[ch][b];
                    gainsChunk.pBuffer[b * channels + ch] = g;
                }
            }
            wns_modules::WNS_Preprocessing preproc;
            preproc.writeBufferToTxt("limiter_gain", gainsChunk);
        } catch (...) {
            // ignore logging errors
        }
    }
    WNS_LOG("Limiter gain values computed and logged in log/limiter_gain.txt");

    return WSN_NO_ERROR;
}

wsn_eF WNS_Limiter::vProcess(const wns_infrastructure::BufferChunk& bufferIn, wns_infrastructure::BufferChunk& bufferOut)
{
    if (!sCoeff.isSet) return WSN_UNSET_LIMITER_PARAMETERS;
    if (bufferIn.samples() == 0 || bufferOut.samples() == 0) return WSN_INVALID_ARGUMENT;
    if (bufferIn.samples() != bufferOut.samples()) return WSN_INVALID_ARGUMENT;

    // compute or refresh gain buffer
    vGetGainBuffer(bufferIn, bufferOut);

    const float *in = bufferIn.data();
    float *out = bufferOut.data();
    int channels = bufferIn.channels;
    size_t frames = bufferIn.frames;
    int down = std::max(1, nDownSampleFactor);
    int D = std::max(0, sCoeff.iDelay); // lookahead in samples
    if (D <= 0) {
        // No lookahead: apply block gain directly
        for (size_t f = 0; f < frames; ++f) {
            size_t blockIdx = f / down;
            for (int ch = 0; ch < channels; ++ch) {
                float g = 1.0f;
                if (blockIdx < pGainBuffer[ch].size()) g = pGainBuffer[ch][blockIdx];
                size_t idx = f * channels + ch;
                out[idx] = in[idx] * g;
            }
        }
    } else {
        // With lookahead D: write current sample into ring, read delayed sample,
        // apply gain computed for future frame (f + D)
        for (size_t f = 0; f < frames; ++f) {
            for (int ch = 0; ch < channels; ++ch) {
                size_t idx = f * channels + ch;
                float x = in[idx];

                // write into circular buffer
                size_t w = writePos[ch] % delayBuffers[ch].size();
                delayBuffers[ch][w] = x;

                // read delayed sample (the sample from D frames ago)
                size_t readPos = (w + 1) % delayBuffers[ch].size();
                float delayedSample = delayBuffers[ch][readPos];

                // determine gain for future frame f + D
                size_t futureFrame = f + (size_t)D;
                size_t gainBlockIdx = futureFrame / down;
                if (pGainBuffer[ch].empty()) {
                    out[idx] = delayedSample;
                } else {
                    if (gainBlockIdx >= pGainBuffer[ch].size()) gainBlockIdx = pGainBuffer[ch].size() - 1;
                    float g = pGainBuffer[ch][gainBlockIdx];
                    out[idx] = delayedSample * g;
                }

                // advance write pointer
                writePos[ch] = (writePos[ch] + 1) % delayBuffers[ch].size();
            }
        }
    }

    return WSN_NO_ERROR;
}

} // namespace wns_modules