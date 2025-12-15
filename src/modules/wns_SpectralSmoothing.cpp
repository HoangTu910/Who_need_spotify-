#include "../inc/modules/wns_SpectralSmoothing.hpp"

namespace wns_modules {

WNS_SpectralSmoothing::WNS_SpectralSmoothing() {}
WNS_SpectralSmoothing::~WNS_SpectralSmoothing() {}

wsn_eF WNS_SpectralSmoothing::vProcess(const wns_infrastructure::BufferChunk& buffer) { (void)buffer; return WSN_NO_ERROR; }

} // namespace wns_modules
