#ifndef _BIQUAD_FILTER_H_
#define _BIQUAD_FILTER_H_

#include "../common/wsn_Common.hpp"
#include "../common/wsn_ErrorCodes.hpp"

namespace wns_Filters
{
struct wns_BiquadCoefficients
{
    double b0;
    double b1;
    double b2;
    double a1;
    double a2;
    double a0;
};

enum wns_BiquadType
{
    LOWPASS,
    HIGHPASS,
    BANDPASS,
    NOTCH,
    PEAKING,
    LOWSHELF,
    HIGHSHELF
};
   
class wns_BiquadFilter 
{
public:
    wns_BiquadFilter();
    ~wns_BiquadFilter();

    wsn_eF eSetFilterType(wns_BiquadType type);
    void vSetFilterParams(double dFc, double dQ, double dGainDB = 0.0, double dSlope = 1.0);

private:
    double dGaindB /* Only for PEAKING and SHELVING EQ filters */, 
           dShelfSlope, 
           dQfactor,
           dBandwidth,
           dOmega,
           dAlpha;
    double dFc, dXn1, dXn2, dYn1, dYn2; /* For difference equation */
    double dA; /* Only for PEAKING and SHELVING EQ filters */
    bool bIsParamSet;
    wns_BiquadCoefficients sCoeffs;
};
}


#endif

