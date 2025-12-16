import os
import numpy as np

# choose writer: scipy is widely available; uncomment soundfile variant if you prefer
import wave
# import soundfile as sf

def make_overshoot_signal(fs=48000, duration_s=2.0):
    t = np.linspace(0.0, duration_s, int(fs * duration_s), endpoint=False)
    # steady tone (low level)
    sig = 0.15 * np.sin(2.0 * np.pi * 440.0 * t)

    # short high-amplitude spike (very short transient)
    spike_start = int(0.5 * fs)
    spike_len = max(1, int(0.0008 * fs))   # ~0.8 ms
    window = np.hanning(spike_len) * 1.0   # scale the spike envelope
    sig[spike_start:spike_start + spike_len] += 2.5 * window

    # short high-frequency burst (a few ms) that may cause overshoot
    burst_start = int(1.2 * fs)
    burst_len = max(1, int(0.005 * fs))    # ~5 ms
    burst_t = np.arange(burst_len) / float(fs)
    burst = 0.9 * np.sin(2.0 * np.pi * 3000.0 * burst_t) * np.hanning(burst_len)
    sig[burst_start:burst_start + burst_len] += burst

    # small DC offset and gentle noise floor to make it realistic
    sig += 0.001 * np.random.randn(sig.size)

    # normalize safely to avoid clipping beyond [-1,1]
    peak = np.max(np.abs(sig))
    if peak > 0.999:
        sig = sig / peak * 0.999

    return sig.astype(np.float32)

if __name__ == '__main__':
    fs = 48000
    sig = make_overshoot_signal(fs=fs, duration_s=2.0)
    os.makedirs('audio', exist_ok=True)
    out_path = 'audio/overshoot_test.wav'
    # write 16-bit PCM using the standard library wave module
    # convert float32 [-1,1] to int16
    int_data = np.int16(np.clip(sig, -1.0, 1.0) * 32767)
    with wave.open(out_path, 'wb') as wf:
        wf.setnchannels(1)
        wf.setsampwidth(2)  # 2 bytes = 16 bits
        wf.setframerate(fs)
        wf.writeframes(int_data.tobytes())
    print('Wrote', out_path)