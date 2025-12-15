#include "../inc/modules/wns_Preprocessing.hpp"
#include <fstream>
#include <cstring>
#include <iostream>

#ifndef HAVE_SNDFILE
#define HAVE_SNDFILE
#endif

// Prefer libsndfile when available; otherwise fall back to the local WAV helpers below.
#ifdef HAVE_SNDFILE
#include <sndfile.h>

static bool readWavFloat(const std::string& path, std::vector<float>& out, int& channels, int& sampleRate) {
	SF_INFO info;
	std::memset(&info, 0, sizeof(info));
	SNDFILE* sf = sf_open(path.c_str(), SFM_READ, &info);
	if (!sf) return false;
	channels = info.channels;
	sampleRate = info.samplerate;
	sf_count_t frames = info.frames;
	out.resize(static_cast<size_t>(frames) * static_cast<size_t>(channels));
	sf_count_t read = sf_read_float(sf, out.data(), static_cast<sf_count_t>(out.size()));
	sf_close(sf);
	return read == static_cast<sf_count_t>(out.size());
}

static bool writeWavFloat(const std::string& path, const std::vector<float>& in, int channels, int sampleRate) {
	SF_INFO info;
	std::memset(&info, 0, sizeof(info));
	info.channels = channels;
	info.samplerate = sampleRate;
	info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
	SNDFILE* sf = sf_open(path.c_str(), SFM_WRITE, &info);
	if (!sf) return false;
	sf_count_t written = sf_write_float(sf, in.data(), static_cast<sf_count_t>(in.size()));
	sf_close(sf);
	return written == static_cast<sf_count_t>(in.size());
}
#else
// Minimal WAV I/O helpers (PCM 16). This is intentionally small — for production
// use a well-tested audio I/O library (libsndfile, dr_wav, etc.).

static bool readWavFloat(const std::string& path, std::vector<float>& out, int& channels, int& sampleRate) {
	std::ifstream f(path, std::ios::binary);
	if (!f) return false;

	char riff[4]; f.read(riff,4);
	if (std::strncmp(riff, "RIFF", 4) != 0) return false;
	f.seekg(22);
	uint16_t ch = 0; f.read(reinterpret_cast<char*>(&ch), sizeof(ch));
	channels = static_cast<int>(ch);
	uint32_t sr = 0; f.read(reinterpret_cast<char*>(&sr), sizeof(sr)); sampleRate = static_cast<int>(sr);
	// Very naive: search for 'data' chunk
	f.seekg(12);
	while (f) {
		char id[4]; f.read(id,4);
		uint32_t sz=0; f.read(reinterpret_cast<char*>(&sz), sizeof(sz));
		if (std::strncmp(id, "data",4)==0) {
			// assume 16-bit PCM
			size_t samples = sz / sizeof(int16_t);
			out.resize(samples);
			for (size_t i=0;i<samples;++i) {
				int16_t s=0; f.read(reinterpret_cast<char*>(&s), sizeof(s));
				out[i] = static_cast<float>(s) / 32768.0f;
			}
			return true;
		} else {
			f.seekg(sz, std::ios::cur);
		}
	}
	return false;
}

static bool writeWavFloat(const std::string& path, const std::vector<float>& in, int channels, int sampleRate) {
	std::ofstream f(path, std::ios::binary);
	if (!f) return false;
	uint32_t dataBytes = static_cast<uint32_t>(in.size() * sizeof(int16_t));
	uint32_t fmtChunkSize = 16;
	uint32_t riffSize = 4 + (8 + fmtChunkSize) + (8 + dataBytes);
	f.write("RIFF",4);
	f.write(reinterpret_cast<const char*>(&riffSize), sizeof(riffSize));
	f.write("WAVE",4);
	// fmt
	f.write("fmt ",4);
	uint16_t audioFormat = 1; // PCM
	uint16_t ch = static_cast<uint16_t>(channels);
	uint32_t byteRate = sampleRate * channels * sizeof(int16_t);
	uint16_t blockAlign = channels * sizeof(int16_t);
	f.write(reinterpret_cast<const char*>(&fmtChunkSize), sizeof(fmtChunkSize));
	f.write(reinterpret_cast<const char*>(&audioFormat), sizeof(audioFormat));
	f.write(reinterpret_cast<const char*>(&ch), sizeof(ch));
	f.write(reinterpret_cast<const char*>(&sampleRate), sizeof(sampleRate));
	f.write(reinterpret_cast<const char*>(&byteRate), sizeof(byteRate));
	f.write(reinterpret_cast<const char*>(&blockAlign), sizeof(blockAlign));
	uint16_t bitsPerSample = 16; f.write(reinterpret_cast<const char*>(&bitsPerSample), sizeof(bitsPerSample));
	// data
	f.write("data",4);
	f.write(reinterpret_cast<const char*>(&dataBytes), sizeof(dataBytes));
	for (size_t i=0;i<in.size();++i) {
		float v = in[i];
		if (v > 1.0f) v = 1.0f; if (v < -1.0f) v = -1.0f;
		int16_t s = static_cast<int16_t>(v * 32767.0f);
		f.write(reinterpret_cast<const char*>(&s), sizeof(s));
	}
	return true;
}
#endif

// Compute RMS (linear) across all samples in BufferChunk
static double computeRMS(const wns_infrastructure::BufferChunk &b) {
    const float *p = b.data();
    size_t N = b.samples();
    if (N == 0) return 0.0;
    double sum = 0.0;
    for (size_t i = 0; i < N; ++i) { double v = p[i]; sum += v*v; }
    return std::sqrt(sum / double(N));
}

// Apply scalar gain in-place
static void applyScalarGain(wns_infrastructure::BufferChunk &buffer, double gain) {
    float *p = buffer.data();
    size_t N = buffer.samples();
    for (size_t i = 0; i < N; ++i) p[i] = static_cast<float>(p[i] * gain);
}

namespace wns_modules {

WNS_Preprocessing::WNS_Preprocessing() {}
WNS_Preprocessing::~WNS_Preprocessing() {}

wsn_eF WNS_Preprocessing::vProcess(const wns_infrastructure::BufferChunk& buffer) { (void)buffer; return WSN_NO_ERROR; }

wsn_eF WNS_Preprocessing::processFile(const std::string& inputPath, const std::string& outputPath) {
	std::vector<float> samples;
	int channels = 0; int sampleRate = 0;
	if (!readWavFloat(inputPath, samples, channels, sampleRate)) {
		std::cerr << "Failed to read input file: " << inputPath << std::endl;
		return WSN_INVALID_ARGUMENT;
	}

	// Create a BufferChunk to hold samples (frames = samples.size()/channels)
	if (channels <= 0) return WSN_INVALID_ARGUMENT;
	size_t frames = samples.size() / static_cast<size_t>(channels);
	wns_infrastructure::BufferChunk chunk(frames, channels, sampleRate);
	// copy interleaved samples into chunk
	chunk.copyFrom(samples.data(), samples.size());

	// call processing (in-place)
	wsn_eF err = vProcess(chunk);
	if (err != WSN_NO_ERROR) {
		std::cerr << "Processing failed with code: " << err << std::endl;
		return err;
	}

	// write back processed buffer
	if (!writeWavFloat(outputPath, chunk.pBuffer, channels, sampleRate)) {
		std::cerr << "Failed to write output file: " << outputPath << std::endl;
		return WSN_INVALID_ARGUMENT;
	}

	return WSN_NO_ERROR;
}

wsn_eF WNS_Preprocessing::readAudioToBuffer(const std::string& inputPath, wns_infrastructure::BufferChunk& buffer) {
	std::vector<float> samples;
	int channels = 0;
	int sampleRate = 0;

	if (!readWavFloat(inputPath, samples, channels, sampleRate)) {
		std::cerr << "readAudioToBuffer: failed to read " << inputPath << std::endl;
		return WSN_INVALID_ARGUMENT;
	}

	if (channels <= 0) return WSN_INVALID_ARGUMENT;
	size_t frames = samples.size() / static_cast<size_t>(channels);

	// Populate buffer
	buffer.frames = frames;
	buffer.channels = channels;
	buffer.sampleRate = sampleRate;
	buffer.pBuffer.resize(samples.size());
	for (size_t i = 0; i < samples.size(); ++i) buffer.pBuffer[i] = static_cast<tFloat>(samples[i]);

	return WSN_NO_ERROR;
}

wsn_eF WNS_Preprocessing::writeBufferToAudio(const std::string& outputPath, const wns_infrastructure::BufferChunk& buffer) {
	if (buffer.frames == 0 || buffer.channels <= 0) return WSN_INVALID_ARGUMENT;
	// Use helper to write pBuffer
	if (!writeWavFloat(outputPath, buffer.pBuffer, buffer.channels, buffer.sampleRate)) {
		std::cerr << "writeBufferToAudio: failed to write " << outputPath << std::endl;
		return WSN_INVALID_ARGUMENT;
	}
	return WSN_NO_ERROR;
}

wsn_eF WNS_Preprocessing::writeBufferToTxt(const std::string &filename, const wns_infrastructure::BufferChunk &buffer)
{
    if (buffer.frames == 0 || buffer.channels <= 0) return WSN_INVALID_ARGUMENT;

	std::string fullPath = "log/" + filename + ".txt";
	std::ofstream f(fullPath);
	if (!f) return WSN_INVALID_ARGUMENT;

	// Write interleaved samples, one per line
	size_t total = buffer.samples();
	for (size_t i = 0; i < total; ++i) {
		f << buffer.pBuffer[i] << '\n';
	}
	f.close();
	return WSN_NO_ERROR;
}

wsn_eF WNS_Preprocessing::compensateGainRMS(const wns_infrastructure::BufferChunk &bufferIn, wns_infrastructure::BufferChunk &bufferOut, double maxBoostDB)
{
    if (bufferIn.frames == 0 || bufferIn.channels <= 0) return WSN_INVALID_ARGUMENT;
    if (bufferIn.samples() != bufferOut.samples()) return WSN_INVALID_ARGUMENT;

    double rmsIn = computeRMS(bufferIn);
    double rmsOut = computeRMS(bufferOut);
    if (rmsOut <= 1e-12 || rmsIn <= 1e-12) return WSN_NO_ERROR; // nothing to do

    double desiredGain = rmsIn / rmsOut;
    double maxBoost = std::pow(10.0, maxBoostDB / 20.0);
    if(desiredGain > maxBoost) desiredGain = maxBoost;

    WNS_LOG("Applying gain: ", desiredGain);
    applyScalarGain(bufferOut, desiredGain);
    return WSN_NO_ERROR;
}
} // namespace wns_modules
