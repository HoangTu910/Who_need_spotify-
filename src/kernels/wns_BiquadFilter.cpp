#include "../inc/kernels/wns_BiquadFilter.hpp"
#include "../utils/wsn_utils.hpp"

namespace wns_Kernels
{
wns_BiquadFilter::wns_BiquadFilter()
{
    // Constructor implementation
    WNS_LOG("wns_BiquadFilter constructor called.");
    // Initialize filter state and params to safe defaults
    dGaindB = 0.0;
    dShelfSlope = 1.0;
    dQfactor = 1.0;
    dBandwidth = 0.0;
    dOmega = 0.0;
    dAlpha = 0.0;
    dFc = 0.0;
    dXn1 = dXn2 = dYn1 = dYn2 = 0.0;
    dA = 1.0;
    bIsParamSet = false;
}

wns_BiquadFilter::~wns_BiquadFilter()
{
    // Destructor implementation
    WNS_LOG("wns_BiquadFilter destructor called.");
}

wsn_eF wns_BiquadFilter::eSetFilterType(wns_BiquadType type)
{
    if(!bIsParamSet) return WSN_UNSET_FILTER_PARAMETERS;

    eFilterType = type;
    dOmega = 2.0 * wns_Common::PI * dFc / wns_Common::SAMPLING_FREQUENCY;
    dAlpha = sin(dOmega) / (2.0 * dQfactor);
    dA = pow(10.0, dGaindB / 40.0);

    switch(type)
    {
        case LOWPASS:
        {
            sCoeffs.b0 = (1.0 - cos(dOmega)) / 2.0;
            sCoeffs.b1 = 1.0 - cos(dOmega);
            sCoeffs.b2 = (1.0 - cos(dOmega)) / 2.0;
            sCoeffs.a0 = 1.0 + dAlpha;
            sCoeffs.a1 = -2.0 * cos(dOmega);
            sCoeffs.a2 = 1.0 - dAlpha;
            break;
        }
        case HIGHPASS:
        {
            sCoeffs.b0 = (1.0 + cos(dOmega)) / 2.0;
            sCoeffs.b1 = -(1.0 + cos(dOmega));
            sCoeffs.b2 = (1.0 + cos(dOmega)) / 2.0;
            sCoeffs.a0 = 1.0 + dAlpha;
            sCoeffs.a1 = -2.0 * cos(dOmega);
            sCoeffs.a2 = 1.0 - dAlpha;
            break;
        }
        case BANDPASS:
        {
            sCoeffs.b0 = dAlpha * dQfactor;
            sCoeffs.b1 = 0.0;
            sCoeffs.b2 = -dAlpha * dQfactor;
            sCoeffs.a0 = 1.0 + dAlpha;
            sCoeffs.a1 = -2.0 * cos(dOmega);
            sCoeffs.a2 = 1.0 - dAlpha;
            break;
        }
        case NOTCH:
        {
            sCoeffs.b0 = 1.0;
            sCoeffs.b1 = -2.0 * cos(dOmega);
            sCoeffs.b2 = 1.0;
            sCoeffs.a0 = 1.0 + dAlpha;
            sCoeffs.a1 = -2.0 * cos(dOmega);
            sCoeffs.a2 = 1.0 - dAlpha;
            break;
        }
        case PEAKING:
        {
            sCoeffs.b0 = 1.0 + dAlpha * dA;
            sCoeffs.b1 = -2.0 * cos(dOmega);
            sCoeffs.b2 = 1.0 - dAlpha * dA;
            sCoeffs.a0 = 1.0 + dAlpha / dA;
            sCoeffs.a1 = -2.0 * cos(dOmega);
            sCoeffs.a2 = 1.0 - dAlpha / dA;
            break;
        }
        case LOWSHELF:
        {
            dAlpha = sin(dOmega) / 2.0 * sqrt((dA + 1/dA) * (1/dShelfSlope - 1) + 2);
            sCoeffs.b0 = dA * ((dA + 1) - (dA - 1) * cos(dOmega) + 2 * sqrt(dA) * dAlpha);
            sCoeffs.b1 = 2 * dA * ((dA - 1) - (dA + 1) * cos(dOmega));
            sCoeffs.b2 = dA * ((dA + 1) - (dA - 1) * cos(dOmega) - 2 * sqrt(dA) * dAlpha);
            sCoeffs.a0 = (dA + 1) + (dA - 1) * cos(dOmega) + 2 * sqrt(dA) * dAlpha;
            sCoeffs.a1 = -2 * ((dA - 1) + (dA + 1) * cos(dOmega));
            sCoeffs.a2 = (dA + 1) + (dA - 1) * cos(dOmega) - 2 * sqrt(dA) * dAlpha;
            break;
        }
        case HIGHSHELF:
        {
            dAlpha = sin(dOmega) / 2.0 * sqrt((dA + 1/dA) * (1/dShelfSlope - 1) + 2);
            sCoeffs.b0 = dA * ((dA + 1) + (dA - 1) * cos(dOmega) + 2 * sqrt(dA) * dAlpha);
            sCoeffs.b1 = -2 * dA * ((dA - 1) + (dA + 1) * cos(dOmega));
            sCoeffs.b2 = dA * ((dA + 1) + (dA - 1) * cos(dOmega) - 2 * sqrt(dA) * dAlpha);
            sCoeffs.a0 = (dA + 1) - (dA - 1) * cos(dOmega) + 2 * sqrt(dA) * dAlpha;
            sCoeffs.a1 = 2 * ((dA - 1) - (dA + 1) * cos(dOmega));
            sCoeffs.a2 = (dA + 1) - (dA - 1) * cos(dOmega) - 2 * sqrt(dA) * dAlpha;
            break;
        }
        default:
            return WSN_UNKNOWN_FILTER_TYPE;
    }
    return WSN_NO_ERROR;
}

void wns_BiquadFilter::vSetFilterParams(double dFc, double dQ, double dGainDB, double dSlope)
{
    this->dFc = dFc;
    this->dQfactor = dQ;
    this->dGaindB = dGainDB;
    this->dShelfSlope = dSlope;
    this->bIsParamSet = true;
}
}

double wns_Kernels::wns_BiquadFilter::sExecute(double dInputSample)
{
    // Direct Form I/II style difference equation (using stored histories)
    double dOutputSample = (sCoeffs.b0 / sCoeffs.a0) * dInputSample +
                           (sCoeffs.b1 / sCoeffs.a0) * dXn1 +
                           (sCoeffs.b2 / sCoeffs.a0) * dXn2 -
                           (sCoeffs.a1 / sCoeffs.a0) * dYn1 -
                           (sCoeffs.a2 / sCoeffs.a0) * dYn2;

    // shift histories: x[n-2] = x[n-1], x[n-1] = x[n]
    dXn2 = dXn1;
    dXn1 = dInputSample;

    // y histories: y[n-2] = y[n-1], y[n-1] = y[n]
    dYn2 = dYn1;
    dYn1 = dOutputSample;

    return dOutputSample;
}

wsn_eF wns_Kernels::wns_BiquadFilter::vProcess(const wns_infrastructure::BufferChunk &bufferIn,
                                               wns_infrastructure::BufferChunk &bufferOut)
{
    if (!bIsParamSet) return WSN_UNSET_FILTER_PARAMETERS;
    WNS_LOG("Called biquad filter");
    std::cout << "------ Filter Params details ------\n"
              << "FilterType = " << eFilterType << "\n"
              << "b0 = " << sCoeffs.b0 << "\n"
              << "b1 = " << sCoeffs.b1 << "\n"
              << "b2 = " << sCoeffs.b2 << "\n"
              << "a0 = " << sCoeffs.a0 << "\n"
              << "a1 = " << sCoeffs.a1 << "\n"
              << "a2 = " << sCoeffs.a2 << std::endl;
    // Process each sample in the buffer
    for (size_t i = 0; i < bufferIn.samples(); ++i) {
        double inputSample = bufferIn.data()[i];
        double outputSample = sExecute(inputSample);
        bufferOut.data()[i] = outputSample;
    }
    return WSN_NO_ERROR;
}
