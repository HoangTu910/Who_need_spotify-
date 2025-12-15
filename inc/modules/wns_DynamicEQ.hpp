#ifndef WNS_DYNAMICEQ_HPP
#define WNS_DYNAMICEQ_HPP

#include "../pipeline/IAudioModules.hpp"

namespace wns_modules {

class WNS_DynamicEQ : public IAudioModule {
public:
	WNS_DynamicEQ();
	~WNS_DynamicEQ();

	void vConfigure();
	wsn_eF vProcess(const wns_infrastructure::BufferChunk& buffer) override;
};

} // namespace wns_modules

#endif // WNS_DYNAMICEQ_HPP
