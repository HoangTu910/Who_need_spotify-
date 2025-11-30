#include "../inc/modules/wns_FFT.hpp"

wns_Modules::wns_FFT::wns_FFT()
{
    WNS_LOG("wns_FFT constructor called.");
}

wns_Modules::wns_FFT::~wns_FFT()
{
    WNS_LOG("wns_FFT destructor called.");
}

wsn_eF wns_Modules::wns_FFT::eComputeFFT(const std::vector<double> &input, std::vector<std::complex<double>> &output)
{
    return wsn_eF();
}

wsn_eF wns_Modules::wns_FFT::eComputeIFFT(const std::vector<std::complex<double>> &input, std::vector<double> &output)
{
    return wsn_eF();
}

void wns_Modules::wns_FFT::vBitReversal(std::vector<std::complex<double>> &data)
{
    // Bit-reversal permutation
    int n = data.size();
    int j = 0;
    
    for (int i = 0; i < n - 1; i++)
    {
        if (i < j)
        {
            std::swap(data[i], data[j]);
        }
        
        int k = n / 2;
        while (k <= j)
        {
            j -= k;
            k /= 2;
        }
        j += k;
    }
}
