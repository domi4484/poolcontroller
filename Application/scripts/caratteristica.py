#!/usr/bin/python3

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#% Riscrittura in python dell'Octave script per trovare i
#% parametri della caratteristica della sonda pt100. Il file
#% "parametri" viene in seguito letto dal programma principale
#% per calcolare la temperatura in funzione del voltaggio

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

import sys

import numpy as np
import matplotlib.pyplot as plt

def main():

    #% Carico i dati misurati (referenza)
    voltage, temperature = np.loadtxt(sys.argv[1], delimiter=';', usecols=(0, 1), unpack=True)

    #% Calcolo della regressione lineare per un polinomio di 2 grado
    a, b, c = np.polyfit(voltage, temperature, 2);
    p = [str(a), str(b), str(c)]
    print(' '.join(p))

    #% Preparo la curva calcolata per essere plottata
    calculated_voltages = np.arange(2.0, 2.5, 0.05);
    calculated_temperatures = c + calculated_voltages*b + calculated_voltages*calculated_voltages*a;

    #% Plot delle due curve
    plt.plot(voltage, temperature, 'o', calculated_voltages, calculated_temperatures, 'b')
    plt.savefig('caratteristica.png')

if __name__ == "__main__":
    main()

quit(0)
