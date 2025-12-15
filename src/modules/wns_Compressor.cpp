#include "../inc/modules/wns_Compressor.hpp"

namespace wns_modules {

// Skeleton implementation — methods left as minimal stubs.

WNS_Compressor::WNS_Compressor() {}
WNS_Compressor::~WNS_Compressor() {}

void WNS_Compressor::vSetThreshold(float) {}
void WNS_Compressor::vSetRatio(float) {}
wsn_eF WNS_Compressor::vProcess(const wns_infrastructure::BufferChunk& buffer) { (void)buffer; return WSN_NO_ERROR; }

} // namespace wns_modules
