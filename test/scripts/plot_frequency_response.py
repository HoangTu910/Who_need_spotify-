import numpy as np
import matplotlib.pyplot as plt
from filters.BiquadFilter import BiquadFilter

def plot_frequency_response(filter_params):
    w, h = BiquadFilter(filter_params).frequency_response()
    plt.plot(w, 20 * np.log10(abs(h)))
    plt.title('Frequency Response')
    plt.xlabel('Frequency (rad/sample)')
    plt.ylabel('Magnitude (dB)')
    plt.grid()
    plt.show()

if __name__ == "__main__":
    filter_params = {
        'type': 'lowpass',
        'frequency': 0.1,
        'Q': 0.707
    }
    plot_frequency_response(filter_params)