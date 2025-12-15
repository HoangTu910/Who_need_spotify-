#include "../inc/modules/wns_Enhancer.hpp"

namespace wns_modules {

WNS_Enhancer::WNS_Enhancer() {}
WNS_Enhancer::~WNS_Enhancer() {}

void WNS_Enhancer::vSetAmount(float) {}
wsn_eF WNS_Enhancer::vProcess(const wns_infrastructure::BufferChunk& buffer) { (void)buffer; return WSN_NO_ERROR; }

} // namespace wns_modules
