#ifndef WNS_LIMITER_HPP
#define WNS_LIMITER_HPP

#include <algorithm>

class WNS_Limiter {
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

    float fGetPeakValue() const;
    float fGetGainReduction() const;
};

#endif // WNS_LIMITER_HPP