#ifndef WNS_LIMITER_HPP
#define WNS_LIMITER_HPP

#include <algorithm>
#include <vector>
#include "../pipeline/IAudioModules.hpp"
#include "../utils/wsn_utils.hpp"

typedef struct {
    float fThreshold;
    float fAttackTime;
    float fReleaseTime;
    int iDelay;
    float fCoeff;
    bool isSet = false;
} limiterCoeff;

namespace wns_modules {

class WNS_Limiter : public IAudioModule {
private:
    float fThreshold;
    float fAttackTime;
    float fReleaseTime;
    float fPeakValue;
    float fGainReduction = 1.0f;
    int nDownSampleFactor;
    float fPeakSignal;

    limiterCoeff sCoeff;

    // per-channel gain buffer (gain per frame, computed at downsampled rate and expanded)
    std::vector<std::vector<float>> pGainBuffer;
    // per-channel state
    std::vector<float> xpeakPerChannel;
    std::vector<float> gPerChannel;
    
    bool bStateInitialized = false;

    // lookahead delay buffers (per-channel) and write pointers
    std::vector<std::vector<float>> delayBuffers;
    std::vector<size_t> writePos;
public:
    WNS_Limiter() = default;
    ~WNS_Limiter() = default;

    wsn_eF vSetLimiterCoeffs(limiterCoeff &coeff);
    void vSetThreshold(float threshold);
    void vSetAttackTime(float attack);
    void vSetReleaseTime(float release);
    void vSetDownSampleFactor(int factor);
    void vDownSampleAndProcess(float* input, float* output, int numSamples);
    // unified processing interface
    wsn_eF vProcess(const wns_infrastructure::BufferChunk& bufferIn, wns_infrastructure::BufferChunk& bufferOut) override;

    float fGetPeakValue() const;
    float fGetGainReduction() const;
    wsn_eF vGetGainBuffer(const wns_infrastructure::BufferChunk& bufferIn, wns_infrastructure::BufferChunk& bufferOut);
};

} // namespace wns_modules

#endif // WNS_LIMITER_HPP