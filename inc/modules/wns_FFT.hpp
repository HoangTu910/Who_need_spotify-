#ifndef _WNS_FFT_H_
#define _WNS_FFT_H_

#include "../common/wsn_Common.hpp"
#include "../common/wsn_ErrorCodes.hpp"
#include "../utils/wsn_utils.hpp"
#include <algorithm> 
#include <complex>

namespace wns_Modules
{
    class wns_FFT
    {
    public:
        wns_FFT();
        ~wns_FFT();

        wsn_eF eComputeFFT(const std::vector<double>& input, std::vector<std::complex<double>>& output);
        wsn_eF eComputeIFFT(const std::vector<std::complex<double>>& input, std::vector<double>& output);
    private:
        void vBitReversal(std::vector<std::complex<double>>& data);
    };
}

#endif
