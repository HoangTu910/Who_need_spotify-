#ifndef WNS_CORRECTIVEEQ_HPP
#define WNS_CORRECTIVEEQ_HPP

#include "../pipeline/IAudioModules.hpp"

namespace wns_modules {

class WNS_CorrectiveEQ : public IAudioModule {
public:
	WNS_CorrectiveEQ();
	~WNS_CorrectiveEQ();

	void vConfigure();
	wsn_eF vProcess(const wns_infrastructure::BufferChunk& bufferIn, wns_infrastructure::BufferChunk& bufferOut) override;
};

} // namespace wns_modules

#endif // WNS_CORRECTIVEEQ_HPP
