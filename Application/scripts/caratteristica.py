#!/usr/bin/python3

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#% Riscrittura in python dell'Octave script per trovare i 
#% parametri della caratteristica della sonda pt100. Il file 
#% "parametri" viene in seguito letto dal programma principale 
#% per calcolare la temperatura in funzione del voltaggio

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

import sqlite3
import numpy as np

BANCA = 'banca_dati.sqlite3'

#% Carico i dati misurati (referenza)
connessione = sqlite3.connect(BANCA)
cursore = connessione.cursor()
voltage = np.array([])
temperature = np.array([])
cursore.execute('SELECT voltage, temperature FROM calibration_data')
for row in cursore:
  voltage = np.r_[voltage, row[0]]
  temperature = np.r_[temperature, row[1]]


#% Calcolo della regressione lineare per un polinomio di 2 grado
a, b, c = np.polyfit(voltage, temperature, 2);
p = [str(a), str(b), str(c)]
print(' '.join(p))

#% Preparo la curva calcolata per essere plottata
#a = p(1)
#b = p(2)
#c = p(3)
#x = linspace(2, 2.5, 20);
#y = c + b.*x + a.*x.*x;

#% Plot delle due curve
#figure(2)
#plot(x, y, T, U, 'o')
#grid on

quit(0)
