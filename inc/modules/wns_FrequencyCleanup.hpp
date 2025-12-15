#ifndef WNS_FREQUENCYCLEANUP_HPP
#define WNS_FREQUENCYCLEANUP_HPP

#include "../pipeline/IAudioModules.hpp"

namespace wns_modules {

class WNS_FrequencyCleanup : public IAudioModule {
public:
	WNS_FrequencyCleanup();
	~WNS_FrequencyCleanup();

	wsn_eF vProcess(const wns_infrastructure::BufferChunk& buffer) override;
};

} // namespace wns_modules

#endif // WNS_FREQUENCYCLEANUP_HPP
