#!/usr/bin/env python3
"""
Plot time-domain signal and magnitude spectrum from a log TXT file produced by
`WNS_Preprocessing::writeBufferToTxt` (one sample per line, interleaved channels).

Usage:
  ./scripts/plot_log.py log/multi_440_880_1760_2000ms.txt --channels 3 --samplerate 48000

Options:
  --file/-f        Path to the log TXT file (required)
  --channels/-c    Number of channels in the interleaved file (default: 1)
  --channel/-C     Channel index to plot (0-based, default: 0)
  --samplerate/-s  Sampling rate in Hz (default: 48000)
  --duration/-d    Duration in seconds to plot from the start (default: entire file)
  --out/-o         Output PNG file to save the plot (optional)

The script plots the waveform (time domain) and the magnitude spectrum (dB) up to Nyquist.
"""

import argparse
import numpy as np
import matplotlib.pyplot as plt
import os


def read_interleaved_txt(path, channels=1):
    # Read all floats from file
    data = np.loadtxt(path, dtype=np.float32)
    if data.size == 0:
        return np.empty((0, channels), dtype=np.float32)
    # If channels == 1, return column vector
    if channels == 1:
        return data.reshape(-1, 1)
    # Ensure length is divisible by channels
    n = data.size // channels
    data = data[:n * channels]
    frames = data.reshape((n, channels))
    return frames


def plot_signal_and_spectrum(frames, samplerate, channel_index=0, duration=None, outpath=None, title=None):
    if frames.size == 0:
        raise RuntimeError("Empty signal")
    total_frames = frames.shape[0]
    if duration is None:
        N = total_frames
    else:
        N = min(total_frames, int(duration * samplerate))
    sig = frames[:N, channel_index]
    t = np.arange(N) / float(samplerate)

    # FFT
    # Use next power of two for nicer resolution
    nfft = 1 << (int(np.ceil(np.log2(N))) if N>0 else 0)
    # zero-pad to at least N
    if nfft < N:
        nfft = N
    window = np.hanning(N)
    sig_win = sig * window
    S = np.fft.rfft(sig_win, n=nfft)
    freqs = np.fft.rfftfreq(nfft, d=1.0 / samplerate)
    mag = np.abs(S) / (np.sum(window) / 2.0)
    mag_db = 20.0 * np.log10(mag + 1e-12)

    # Figure
    fig, axes = plt.subplots(2, 1, figsize=(10, 6))
    if title:
        fig.suptitle(title)

    axes[0].plot(t, sig, lw=0.8)
    axes[0].set_xlabel('Time (s)')
    axes[0].set_ylabel('Amplitude')
    axes[0].grid(True)

    axes[1].plot(freqs, mag_db, lw=0.8)
    axes[1].set_xlabel('Frequency (Hz)')
    axes[1].set_ylabel('Magnitude (dB)')
    axes[1].set_xlim(0, samplerate/2)
    axes[1].grid(True)

    plt.tight_layout(rect=[0, 0, 1, 0.96])
    if outpath:
        plt.savefig(outpath, dpi=150)
        print('Saved plot to', outpath)
    else:
        plt.show()


def main():
    p = argparse.ArgumentParser(description='Plot signal and spectrum from interleaved log TXT')
    p.add_argument('file', help='Path to the log TXT file')
    p.add_argument('-c', '--channels', type=int, default=1, help='Number of channels (default 1)')
    p.add_argument('-C', '--channel', type=int, default=0, help='Channel index to plot (0-based)')
    p.add_argument('-s', '--samplerate', type=int, default=48000, help='Sampling rate in Hz (default 48000)')
    p.add_argument('-d', '--duration', type=float, default=None, help='Duration in seconds to plot (default all)')
    p.add_argument('-o', '--out', default=os.path.join('test', 'log_plot.png'), help='Optional output PNG path (default: test/log_plot.png)')
    args = p.parse_args()

    if not os.path.exists(args.file):
        print('File not found:', args.file)
        return

    frames = read_interleaved_txt(args.file, channels=args.channels)
    if frames.shape[1] <= args.channel:
        print('Channel index', args.channel, 'out of range for channels=', args.channels)
        return

    title = os.path.basename(args.file)
    plot_signal_and_spectrum(frames, args.samplerate, channel_index=args.channel, duration=args.duration, outpath=args.out, title=title)


if __name__ == '__main__':
    main()
