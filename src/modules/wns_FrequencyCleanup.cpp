#include "../inc/modules/wns_FrequencyCleanup.hpp"

namespace wns_modules {

WNS_FrequencyCleanup::WNS_FrequencyCleanup() {}
WNS_FrequencyCleanup::~WNS_FrequencyCleanup() {}

wsn_eF WNS_FrequencyCleanup::vProcess(const wns_infrastructure::BufferChunk& buffer) { (void)buffer; return WSN_NO_ERROR; }

} // namespace wns_modules
