#ifndef WNS_NOISEREDUCTION_HPP
#define WNS_NOISEREDUCTION_HPP

#include "../pipeline/IAudioModules.hpp"

namespace wns_modules {

class WNS_NoiseReduction : public IAudioModule {
public:
	WNS_NoiseReduction();
	~WNS_NoiseReduction();

	void vConfigure();
	wsn_eF vProcess(const wns_infrastructure::BufferChunk& bufferIn, wns_infrastructure::BufferChunk& bufferOut) override;
};

} // namespace wns_modules

#endif // WNS_NOISEREDUCTION_HPP
