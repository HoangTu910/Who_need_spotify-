#ifndef WNS_ENHANCER_HPP
#define WNS_ENHANCER_HPP

#include "../pipeline/IAudioModules.hpp"

namespace wns_modules {

class WNS_Enhancer : public IAudioModule {
public:
	WNS_Enhancer();
	~WNS_Enhancer();

	void vSetAmount(float amount);
	wsn_eF vProcess(const wns_infrastructure::BufferChunk& bufferIn, wns_infrastructure::BufferChunk& bufferOut) override;
};

} // namespace wns_modules

#endif // WNS_ENHANCER_HPP
