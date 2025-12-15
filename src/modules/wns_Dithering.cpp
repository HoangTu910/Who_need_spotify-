#include "../inc/modules/wns_Dithering.hpp"

namespace wns_modules {

WNS_Dithering::WNS_Dithering() {}
WNS_Dithering::~WNS_Dithering() {}

wsn_eF WNS_Dithering::vProcess(const wns_infrastructure::BufferChunk& buffer) { (void)buffer; return WSN_NO_ERROR; }

} // namespace wns_modules
