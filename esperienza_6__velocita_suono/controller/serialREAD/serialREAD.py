
import os
from serial import Serial
import ROOT as root

from ROOT import TH1D, TFile

from datetime import datetime

import numpy as np
import matplotlib.pyplot as plt

import matplotlib

fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2,2)


devSERIAL = Serial(port='/dev/tty.usbmodem143301', baudrate=9600)

fNdataread = 50
timeRUN_onrise = np.zeros((2, fNdataread))
timeRUN_onfall = np.zeros((2, fNdataread))

BASE_PATH = os.getcwd()

datafile = open(f'{BASE_PATH}/../../misc/DATA_serial_RENAME.txt', 'w')

while(root.kTRUE):
    sline = devSERIAL.readline().decode('utf-8');
    print(sline)
    if(sline=='** RUN data result **\r\n'):
        for j in range(2):
            for i in range(50):
                data = devSERIAL.readline().decode('utf-8')
                print(data)
                timeRUN_onrise[j][i] = int(data)
        for j in range(2):
            for i in range(50):
                data = devSERIAL.readline().decode('utf-8')
                print(data)
                timeRUN_onfall[j][i] = int(data)
        break

ax1.hist(timeRUN_onfall[0], bins=20)
ax2.hist(timeRUN_onfall[1], bins=20)
ax3.hist(timeRUN_onrise[0], bins=20)
ax4.hist(timeRUN_onrise[1], bins=20)

for j in range(2):
    for i in range(50):
        datafile.write(f'{str(timeRUN_onrise[j][i])}\n')
for j in range(2):
    for i in range(50):
        datafile.write(f'{str(timeRUN_onfall[j][i])}\n')

plt.show()



