#ifndef WNS_SPECTRALSMOOTHING_HPP
#define WNS_SPECTRALSMOOTHING_HPP

#include "../pipeline/IAudioModules.hpp"

namespace wns_modules {

class WNS_SpectralSmoothing : public IAudioModule {
public:
	WNS_SpectralSmoothing();
	~WNS_SpectralSmoothing();

	wsn_eF vProcess(const wns_infrastructure::BufferChunk& bufferIn, wns_infrastructure::BufferChunk& bufferOut) override;
};

} // namespace wns_modules

#endif // WNS_SPECTRALSMOOTHING_HPP
