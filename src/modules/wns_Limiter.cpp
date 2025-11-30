#include "../inc/modules/wns_Limiter.hpp"

void WNS_Limiter::vDownSampleAndProcess(float *input, float *output, int numSamples)
{
    int downSampledSamples = numSamples / nDownSampleFactor;
    for (int i = 0; i < downSampledSamples; ++i) {
        float sample = input[i * nDownSampleFactor];
        fPeakSignal = std::max(fPeakSignal * (1.0f - fReleaseTime), std::abs(sample));
        if (fPeakSignal > fThreshold) {
            fGainReduction = fAttackTime * (fPeakSignal - fThreshold);
            sample *= (fThreshold / fPeakSignal);
        } else {
            fGainReduction = 0.0f;
        }
        output[i] = sample;
        fPeakValue = std::max(fPeakValue, fPeakSignal);
    }
}