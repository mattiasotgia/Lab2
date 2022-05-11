import glob
import time
import ROOT as root
import os, sys

from os.path import join
from serial import Serial, SerialException

import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt

from datetime import datetime

SRL3_DICT = {
    'start_remote': '?STR',
    'start_run_confirm': '*RUN',
    'end_run_confirm': '*STP'
}

BASE_DATA_PATH = '/Users/massimosotgia/Desktop/uni_at_DIFI/Lab2/esperienza_7__effetto_hall/dati'

def setserial(port, baudrate = 9600):
    devSERIAL = Serial(port=port, baudrate=baudrate)
    try:
        devSERIAL.open() 
    except SerialException:
        print('Unable to establish serial comm. on port {}'.format(devSERIAL.port))
    except:
        print('Unknown error!')

def serial_write(serial: Serial, command):
    '''
        # Args:
        - `serial`: serial port
        - `command`: command in string or char array
    '''
    
    try:
        serial.write('{}\n'.format(command.encode('utf-8')))
    except:
        print('Failed command "{} to serial {serial}"'.format(command, serial=serial.port))
    time.sleep(0.1)



def main():
    
    Bpol = ['Bpos', 'Bneg', 'B?']
    today: str = datetime.today().strftime('%Y%m%d_%H.%M.%S.%f');
    
    if ( len(sys.argv) > 1):
        if (sys.argv[1] == '0' or sys.argv[1] == '1'):
            output_file: str = 'dataRUN_{}_{}_HALL.root'.format(Bpol[int(sys.argv[1])], today)
    else:
        output_file: str = 'dataRUN_B?_{}_HALL.root'.format(today)
    
    _file0 = root.TFile(join(BASE_DATA_PATH, output_file), 'RECREATE')

    _data0ttree = root.TTree('data_{}'.format(Bpol[int(sys.argv[1]) if len(sys.argv) > 1 else 2]), 'RUN data E7/hall')
    


    return
