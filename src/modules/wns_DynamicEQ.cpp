#include "../inc/modules/wns_DynamicEQ.hpp"

namespace wns_modules {

WNS_DynamicEQ::WNS_DynamicEQ() {}
WNS_DynamicEQ::~WNS_DynamicEQ() {}

void WNS_DynamicEQ::vConfigure() {}
wsn_eF WNS_DynamicEQ::vProcess(const wns_infrastructure::BufferChunk& bufferIn, wns_infrastructure::BufferChunk& bufferOut) { (void)bufferIn; return WSN_NO_ERROR; }

} // namespace wns_modules
