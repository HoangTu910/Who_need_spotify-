#include <iostream>
#include <vector>
#include <complex>
#include <cmath>

#include "../../inc/kernels/wns_FFT.hpp"

int main()
{
    using namespace wns_Kernels;

    wns_FFT fft;

    std::vector<int> sizes = {8, 16, 32, 64};

    for (int n : sizes)
    {
        std::vector<double> input(n);
        for (int i = 0; i < n; ++i)
            input[i] = (i == 0) ? 1.0 : std::sin(2.0 * M_PI * i / n);

        std::vector<std::complex<double>> spectrum;
        auto r1 = fft.eComputeFFT(input, spectrum);
        if (r1 != WSN_NO_ERROR) {
            std::cerr << "FFT failed for size " << n << " with error " << r1 << std::endl;
            return 2;
        }

        std::vector<double> output;
        auto r2 = fft.eComputeIFFT(spectrum, output);
        if (r2 != WSN_NO_ERROR) {
            std::cerr << "IFFT failed for size " << n << " with error " << r2 << std::endl;
            return 3;
        }

        double maxErr = 0.0;
        for (int i = 0; i < n; ++i)
            maxErr = std::max(maxErr, std::abs(output[i] - input[i]));

        std::cout << "Size " << n << " round-trip max error: " << maxErr << std::endl;
        if (maxErr > 1e-6) {
            std::cerr << "Round-trip error too large for size " << n << std::endl;
            return 4;
        }
    }

    std::cout << "FFT/IFFT unit tests passed." << std::endl;
    return 0;
}
