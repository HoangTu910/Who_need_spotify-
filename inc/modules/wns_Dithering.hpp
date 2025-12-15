#ifndef WNS_DITHERING_HPP
#define WNS_DITHERING_HPP

#include "../pipeline/IAudioModules.hpp"

namespace wns_modules {

class WNS_Dithering : public IAudioModule {
public:
	WNS_Dithering();
	~WNS_Dithering();

	wsn_eF vProcess(const wns_infrastructure::BufferChunk& buffer) override;
};

} // namespace wns_modules

#endif // WNS_DITHERING_HPP
