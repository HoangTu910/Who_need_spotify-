#include "../inc/modules/wns_TonalShaping.hpp"

namespace wns_modules {

WNS_TonalShaping::WNS_TonalShaping() {}
WNS_TonalShaping::~WNS_TonalShaping() {}

void WNS_TonalShaping::vSetAmount(float) {}
wsn_eF WNS_TonalShaping::vProcess(const wns_infrastructure::BufferChunk& bufferIn, wns_infrastructure::BufferChunk& bufferOut) { (void)bufferIn; return WSN_NO_ERROR; }

} // namespace wns_modules
