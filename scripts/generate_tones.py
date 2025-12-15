#!/usr/bin/env python3
"""
Generate test tone WAV files (mono) with single and multiple frequencies.
Produces files under audio/ with sampling rate 48000.
"""
import os
import math
import wave
import struct

OUT_DIR = os.path.join(os.path.dirname(__file__), '..', 'audio')
SAMPLE_RATE = 48000

SINGLE_TONES = [220.0, 440.0, 880.0, 1760.0]  # A3, A4, A5, A6
MULTI_TONES = [ [220.0, 440.0], [440.0, 880.0, 1760.0] ]
DURATIONS = [1.0, 2.0]
AMPLITUDE = 0.6

os.makedirs(OUT_DIR, exist_ok=True)

def write_wav(path, samples, sample_rate=SAMPLE_RATE):
    with wave.open(path, 'wb') as wf:
        wf.setnchannels(1)
        wf.setsampwidth(2)  # 16-bit
        wf.setframerate(sample_rate)
        frames = b''.join(struct.pack('<h', int(max(-1.0, min(1.0, s)) * 32767)) for s in samples)
        wf.writeframes(frames)


def gen_sine(freq, length_s, sr=SAMPLE_RATE):
    N = int(length_s * sr)
    return [AMPLITUDE * math.sin(2.0 * math.pi * freq * t / sr) for t in range(N)]


def gen_multi(freqs, length_s, sr=SAMPLE_RATE):
    N = int(length_s * sr)
    samples = [0.0] * N
    for f in freqs:
        for t in range(N):
            samples[t] += (AMPLITUDE / len(freqs)) * math.sin(2.0 * math.pi * f * t / sr)
    return samples


if __name__ == '__main__':
    created = []
    for d in DURATIONS:
        for f in SINGLE_TONES:
            name = f'single_{int(f)}hz_{int(d*1000)}ms.wav'
            path = os.path.join(OUT_DIR, name)
            samples = gen_sine(f, d)
            write_wav(path, samples)
            created.append(path)
        for freqs in MULTI_TONES:
            name = 'multi_' + '_'.join(str(int(f)) for f in freqs) + f'_{int(d*1000)}ms.wav'
            path = os.path.join(OUT_DIR, name)
            samples = gen_multi(freqs, d)
            write_wav(path, samples)
            created.append(path)
    print('Created files:')
    for p in created:
        print(' -', p)
