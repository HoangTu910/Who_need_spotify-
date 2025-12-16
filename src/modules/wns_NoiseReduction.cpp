#include "../inc/modules/wns_NoiseReduction.hpp"

namespace wns_modules {

WNS_NoiseReduction::WNS_NoiseReduction() {}
WNS_NoiseReduction::~WNS_NoiseReduction() {}

void WNS_NoiseReduction::vConfigure() {}
wsn_eF WNS_NoiseReduction::vProcess(const wns_infrastructure::BufferChunk& bufferIn, wns_infrastructure::BufferChunk& bufferOut) { (void)bufferIn; return WSN_NO_ERROR; }

} // namespace wns_modules
