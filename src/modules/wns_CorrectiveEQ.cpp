#include "../inc/modules/wns_CorrectiveEQ.hpp"

namespace wns_modules {

WNS_CorrectiveEQ::WNS_CorrectiveEQ() {}
WNS_CorrectiveEQ::~WNS_CorrectiveEQ() {}

void WNS_CorrectiveEQ::vConfigure() {}
wsn_eF WNS_CorrectiveEQ::vProcess(const wns_infrastructure::BufferChunk& buffer) { (void)buffer; return WSN_NO_ERROR; }

} // namespace wns_modules
