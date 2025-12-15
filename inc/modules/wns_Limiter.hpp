#ifndef WNS_LIMITER_HPP
#define WNS_LIMITER_HPP

#include <algorithm>
#include "../pipeline/IAudioModules.hpp"

namespace wns_modules {

class WNS_Limiter : public IAudioModule {
private:
    float fThreshold;
    float fAttackTime;
    float fReleaseTime;
    float fPeakValue;
    float fGainReduction;
    int nDownSampleFactor;
    float fPeakSignal;
public:
    WNS_Limiter();
    ~WNS_Limiter();

    void vSetThreshold(float threshold);
    void vSetAttackTime(float attack);
    void vSetReleaseTime(float release);
    void vSetDownSampleFactor(int factor);
    void vDownSampleAndProcess(float* input, float* output, int numSamples);
    // unified processing interface
    wsn_eF vProcess(const wns_infrastructure::BufferChunk& buffer) override;

    float fGetPeakValue() const;
    float fGetGainReduction() const;
};

} // namespace wns_modules

#endif // WNS_LIMITER_HPP