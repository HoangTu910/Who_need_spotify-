#include "../inc/kernels/wns_FFT.hpp"

wns_Kernels::wns_FFT::wns_FFT()
{
    WNS_LOG("wns_FFT constructor called.");
}

wns_Kernels::wns_FFT::~wns_FFT()
{
    WNS_LOG("wns_FFT destructor called.");
}

wsn_eF wns_Kernels::wns_FFT::eComputeFFT(const std::vector<double> &input, std::vector<std::complex<double>> &output)
{

    const int n = static_cast<int>(input.size());
    if (n <= 0)
    {
        WNS_ERROR("eComputeFFT: input size must be > 0");
        return WSN_INVALID_ARGUMENT;
    }

    // require power-of-two length for this implementation
    if ((n & (n - 1)) != 0)
    {
        WNS_ERROR("eComputeFFT: input size must be a power of two");
        return WSN_INVALID_ARGUMENT;
    }

    try
    {
        output.resize(n);
    }
    catch (...) {
        WNS_ERROR("eComputeFFT: failed to allocate output buffer");
        return WSN_OUT_OF_MEMORY;
    }

    // copy input into complex buffer (real part = sample)
    for (int i = 0; i < n; ++i) {
        output[i] = std::complex<double>(input[i], 0.0);
    }

    // bit-reversal permutation
    vBitReversal(output);

    // iterative Cooley-Tukey radix-2 FFT
    for (int len = 2; len <= n; len <<= 1)
    {
        double angle = -2.0 * wns_Common::PI / static_cast<double>(len);
        std::complex<double> wlen(std::cos(angle), std::sin(angle));

        for (int i = 0; i < n; i += len)
        {
            std::complex<double> w(1.0, 0.0);
            int half = len >> 1;
            for (int j = 0; j < half; ++j)
            {
                std::complex<double> u = output[i + j];
                std::complex<double> v = output[i + j + half] * w;
                output[i + j] = u + v;
                output[i + j + half] = u - v;
                w *= wlen;
            }
        }
    }

    return WSN_NO_ERROR;
}

wsn_eF wns_Kernels::wns_FFT::eComputeIFFT(const std::vector<std::complex<double>> &input, std::vector<double> &output)
{
    const int n = static_cast<int>(input.size());
    if (n <= 0)
    {
        WNS_ERROR("eComputeIFFT: input size must be > 0");
        return WSN_INVALID_ARGUMENT;
    }

    if ((n & (n - 1)) != 0)
    {
        WNS_ERROR("eComputeIFFT: input size must be a power of two");
        return WSN_INVALID_ARGUMENT;
    }

    std::vector<std::complex<double>> buf;
    try {
        buf = input; // copy
    } catch (...) {
        WNS_ERROR("eComputeIFFT: failed to copy input buffer");
        return WSN_OUT_OF_MEMORY;
    }

    // bit-reversal permutation
    vBitReversal(buf);

    // iterative inverse FFT (same as FFT but with positive angle)
    for (int len = 2; len <= n; len <<= 1)
    {
        double angle = 2.0 * wns_Common::PI / static_cast<double>(len);
        std::complex<double> wlen(std::cos(angle), std::sin(angle));

        for (int i = 0; i < n; i += len)
        {
            std::complex<double> w(1.0, 0.0);
            int half = len >> 1;
            for (int j = 0; j < half; ++j)
            {
                std::complex<double> u = buf[i + j];
                std::complex<double> v = buf[i + j + half] * w;
                buf[i + j] = u + v;
                buf[i + j + half] = u - v;
                w *= wlen;
            }
        }
    }

    // scale by 1/n and extract real part
    try {
        output.resize(n);
    } catch (...) {
        WNS_ERROR("eComputeIFFT: failed to allocate output buffer");
        return WSN_OUT_OF_MEMORY;
    }

    for (int i = 0; i < n; ++i) {
        output[i] = buf[i].real() / static_cast<double>(n);
    }

    return WSN_NO_ERROR;
}

void wns_Kernels::wns_FFT::vBitReversal(std::vector<std::complex<double>> &data)
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
