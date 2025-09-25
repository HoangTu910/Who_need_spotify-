#include "../inc/filters/wns_BiquadFilter.hpp"
#include "../utils/wsn_utils.hpp"

namespace wns_Filters
{
wns_BiquadFilter::wns_BiquadFilter()
{
    // Constructor implementation
}

wns_BiquadFilter::~wns_BiquadFilter()
{
    // Destructor implementation
}

wsn_eF wns_BiquadFilter::vSetFilterType(wns_BiquadType type)
{
    if(!bIsParamSet)
    {
        return WSN_UNSET_PARAMETERS;
    }

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

wsn_eF wns_BiquadFilter::vSetFilterParams(double dFc, double dQ, double dGainDB, double dSlope)
{
    this->dFc = dFc;
    this->dQfactor = dQ;
    this->dGaindB = dGainDB;
    this->dShelfSlope = dSlope;
    this->bIsParamSet = true;
    return WSN_NO_ERROR;
}
}