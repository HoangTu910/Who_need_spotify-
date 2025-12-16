#ifndef WNS_REVERB_HPP
#define WNS_REVERB_HPP

#include "../pipeline/IAudioModules.hpp"

namespace wns_modules {

class WNS_Reverb : public IAudioModule {
public:
	WNS_Reverb();
	~WNS_Reverb();

	void vSetRoomSize(float size);
	wsn_eF vProcess(const wns_infrastructure::BufferChunk& bufferIn, wns_infrastructure::BufferChunk& bufferOut) override;
};

} // namespace wns_modules

#endif // WNS_REVERB_HPP
