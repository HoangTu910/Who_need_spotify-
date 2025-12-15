#include "../inc/modules/wns_DynamicEQ.hpp"

namespace wns_modules {

WNS_DynamicEQ::WNS_DynamicEQ() {}
WNS_DynamicEQ::~WNS_DynamicEQ() {}

void WNS_DynamicEQ::vConfigure() {}
wsn_eF WNS_DynamicEQ::vProcess(const wns_infrastructure::BufferChunk& buffer) { (void)buffer; return WSN_NO_ERROR; }

} // namespace wns_modules
