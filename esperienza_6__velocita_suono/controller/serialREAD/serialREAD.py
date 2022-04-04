

from serial import Serial
import ROOT as root

from ROOT import TH1D, TFile

import numpy as np
import matplotlib.pyplot as plt

import matplotlib



ss = Serial(port='/dev/tty.usbmodem141301', baudrate=9600)
# data_RUN = TH1D("dataRUN", "")

fNdataread = 50
timeRUN_onrise = np.zeros((2, fNdataread))
timeRUN_onfall = np.zeros((2, fNdataread))

datafile = open('../misc/DATA_serial.txt', 'a')

while(root.kTRUE):
    sline = ss.readline().decode('utf-8');
    print(sline)
    if(sline=='** RUN data result **\r\n'):
        for j in range(2):
            for i in range(50):
                data = ss.readline().decode('utf-8')
                print(data)
                timeRUN_onrise[j][i] = data
        for j in range(2):
            for i in range(50):
                data = ss.readline().decode('utf-8')
                print(data)
                timeRUN_onfall[j][i] = data[j][i] = data
        break




