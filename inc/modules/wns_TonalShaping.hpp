#ifndef WNS_TONALSHAPING_HPP
#define WNS_TONALSHAPING_HPP

#include "../pipeline/IAudioModules.hpp"

namespace wns_modules {

class WNS_TonalShaping : public IAudioModule {
public:
	WNS_TonalShaping();
	~WNS_TonalShaping();

	void vSetAmount(float amount);
	wsn_eF vProcess(const wns_infrastructure::BufferChunk& buffer) override;
};

} // namespace wns_modules

#endif // WNS_TONALSHAPING_HPP
