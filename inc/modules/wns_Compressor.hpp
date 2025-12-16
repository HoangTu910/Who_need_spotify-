#ifndef WNS_COMPRESSOR_HPP
#define WNS_COMPRESSOR_HPP

#include "../pipeline/IAudioModules.hpp"

namespace wns_modules {

class WNS_Compressor : public IAudioModule {
public:
	WNS_Compressor();
	~WNS_Compressor();

	void vSetThreshold(float threshold);
	void vSetRatio(float ratio);
	wsn_eF vProcess(const wns_infrastructure::BufferChunk& bufferIn, wns_infrastructure::BufferChunk& bufferOut) override;
};

} // namespace wns_modules

#endif // WNS_COMPRESSOR_HPP
