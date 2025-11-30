#include "../inc/filters/wns_BiquadFilter.hpp"
#include "../inc/modules/wns_FFT.hpp"

int main()
{
    // Example usage of wns_BiquadFilter
    wns_Filters::wns_BiquadFilter biquadFilter;
    biquadFilter.vSetFilterParams(1000.0, 0.707, 0.0, 1.0);
    biquadFilter.eSetFilterType(wns_Filters::LOWPASS);

    double inputSample = 0.5; // Example input sample
    double outputSample = biquadFilter.sExecute(inputSample);

    std::cout << "Input Sample: " << inputSample << ", Output Sample: " << outputSample << std::endl;

    return 0;
}