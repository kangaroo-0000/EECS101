import numpy as np 
import matplotlib.pyplot as plt
import sys

# Program takes in one argument, path to the image file to be plotted, from command line and plots its histogram.

def plot_image_histogram(filename):
    with open(filename, 'rb') as ifile:
        imagearray = np.array(bytearray(ifile.read()))	
        plt.hist(imagearray)
        plt.xticks(np.arange(0, 256, 16))
        plt.show()

plot_image_histogram(sys.argv[1])
