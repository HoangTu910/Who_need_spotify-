#include "../inc/modules/wns_Reverb.hpp"

namespace wns_modules {

WNS_Reverb::WNS_Reverb() {}
WNS_Reverb::~WNS_Reverb() {}

void WNS_Reverb::vSetRoomSize(float) {}
wsn_eF WNS_Reverb::vProcess(const wns_infrastructure::BufferChunk& buffer) { (void)buffer; return WSN_NO_ERROR; }

} // namespace wns_modules
