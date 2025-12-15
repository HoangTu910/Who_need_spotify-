#ifndef WNS_PREPROCESSING_HPP
#define WNS_PREPROCESSING_HPP

#include "../pipeline/IAudioModules.hpp"
#include <string>
#include <vector>

namespace wns_modules {

class WNS_Preprocessing : public IAudioModule {
public:
	WNS_Preprocessing();
	~WNS_Preprocessing();

	wsn_eF vProcess(const wns_infrastructure::BufferChunk& buffer) override;

	// High-level helper: load an audio file, process it through this module, and write output.
	// Non-realtime: performs file I/O and should be called from a non-realtime thread.
	wsn_eF processFile(const std::string& inputPath, const std::string& outputPath);
    wsn_eF readAudioToBuffer(const std::string& inputPath, wns_infrastructure::BufferChunk& buffer);
    wsn_eF writeBufferToAudio(const std::string& outputPath, const wns_infrastructure::BufferChunk& buffer);
	// Write buffer values into a text file under log/<filename>.txt (one sample per line).
	wsn_eF writeBufferToTxt(const std::string& filename, const wns_infrastructure::BufferChunk& buffer);
};

} // namespace wns_modules

#endif // WNS_PREPROCESSING_HPP
