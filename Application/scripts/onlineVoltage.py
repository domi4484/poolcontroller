#!/usr/bin/python3

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#% Script per visualizzare il grafico del voltaggio corrente

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

import numpy as np
import matplotlib.pyplot as plt


voltage = np.loadtxt('VoltageData.dat')

#% Preparo la curva calcolata per essere plottata
#% Plot delle due curve
#figure(2)

plt.axis([0, 20, 2.0, 2.5])
ax = plt.gca()
ax.set_autoscale_on(False)

plt.plot(voltage, 'o', voltage, 'b')
plt.savefig('voltage.png')
#grid on


  

quit(0)
