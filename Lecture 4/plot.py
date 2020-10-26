#! /usr/bin/env python

import sys
import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

def line_1st(x, a, b):
    return a*x + b

def line_2nd(x, a, b, c):
    return a*x**2 + b*x + c

def parse_performance_log(path):
    time, size = [], []
    with open(path) as f:
        for line in f:
            items = line.split()
            size.append(int(items[0]))
            time.append(list(map(float, items[1:])))
    return (size, np.array(time))

def draw_performance_plot(size, time, logx, logy):
    plt.figure(figsize=(8, 6))
    x = np.logspace(np.log10(size[0]), np.log10(size[-1]), 100)
    nplots = time.shape[1]

    for i in range(nplots):
        plt.plot(size, time[:, i], linestyle='none', marker='o', markersize=6)

    # fitfcn = line_1st
    fitfcn = line_2nd
    for i in range(nplots):
        popt, _ = curve_fit(fitfcn, size, time[:, i])
        print(popt)
        plt.plot(x, fitfcn(x, *popt))

    if logx:
        plt.semilogx()
    if logy:
        plt.semilogy()
    plt.grid()
    plt.tight_layout()
    plt.show()

def make_plot(path, logx, logy):
    size, time = parse_performance_log(path)
    draw_performance_plot(size, time, logx, logy)

if __name__ == '__main__':
    try:
        print(sys.argv)
        logx = 'logx' in sys.argv
        logy = 'logy' in sys.argv
        make_plot(sys.argv[1], logx, logy)
    except ValueError:
        print('Usage: ./plot.py path [logx] [logy]')
