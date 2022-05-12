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
    return devSERIAL

def serial_write(serial: Serial, command):
    '''
        # Args:
        - `serial`: serial port
        - `command`: command in string or char array
    '''
    serial.write(bytes(f'{command}\n', encoding='utf-8'))
    time.sleep(0.1)



def main():
    
    Bpol = ['Bpos', 'Bneg', 'B?']
    today: str = datetime.today().strftime('%Y%m%d_%H.%M.%S.%f');
    output_file: str = 'dataRUN_{}_HALL.root'.format(today)

    
    _file0 = root.TFile(join(BASE_DATA_PATH, output_file), 'RECREATE')

    

    # root.gInterpreter.ProcessLine('''
    #     struct Event {
    #         int ID;
    #         int RUN;
    #         double V_gen_lo[12][100];
    #         double V_gen_hi[12][100];
    #         double V_out_lo[12][100];
    #         double V_out_hi[12][100];
    #     };
    # ''')

    devSERIAL = setserial(port='/dev/tty.usbmodem141401')

    M = 12
    N = 100

    while(True):
        if(devSERIAL.readline().decode('utf-8') == '*RUN\n'):
            print('serial comm. to START data aquisition\n')
            line = devSERIAL.readline().decode('utf-8')
            if(line == '*BPOS'):
                _data0ttreeBpos = root.TTree('data_BPOS', 'RUN data E7/hall: positive B')
                _data0ttreeBpos.Branch('event', root.Event())
                for i in range(M):
                    if(devSERIAL.readline().decode('utf-8')=='*CYCLESTART'):
                        pass
                    print(devSERIAL.readline().decode('utf-8'))
                    for j in range(N):
                        root.Event.ID = 0 # Bpos
                        root.Event.RUN = i

                        V_out_hi[i][j] = float(devSERIAL.readline().decode('utf-8'))
                        print(V_out_hi[i][j])
                        V_gen_hi[i][j] = float(devSERIAL.readline().decode('utf-8'))
                        print(V_gen_hi[i][j])
                        V_out_lo[i][j] = float(devSERIAL.readline().decode('utf-8'))
                        print(V_out_lo[i][j])
                        V_gen_lo[i][j] = float(devSERIAL.readline().decode('utf-8'))
                        print(V_gen_lo[i][j])
                    print(devSERIAL.readline().decode('utf-8'))
                    _data0ttreeBpos.Fill()
            else:
                _data0ttreeBneg = root.TTree('data_BNEG', 'RUN data E7/hall: negative B')
                _data0ttreeBneg.Branch('event', root.Event())
                for i in range(M):
                    if(devSERIAL.readline().decode('utf-8')=='*CYCLESTART'):
                        pass
                    print(devSERIAL.readline().decode('utf-8'))
                    for j in range(N):
                        root.Event.ID = 1 # Bneg
                        root.Event.RUN = i

                        root.Event.V_out_hi[i][j] = float(devSERIAL.readline().decode('utf-8'))
                        print(root.Event.V_out_hi[i][j])
                        root.Event.V_gen_hi[i][j] = float(devSERIAL.readline().decode('utf-8'))
                        print(root.Event.V_gen_hi[i][j])
                        root.Event.V_out_lo[i][j] = float(devSERIAL.readline().decode('utf-8'))
                        print(root.Event.V_out_lo[i][j])
                        root.Event.V_gen_lo[i][j] = float(devSERIAL.readline().decode('utf-8'))
                        print(root.Event.V_gen_lo[i][j])
                    print(devSERIAL.readline().decode('utf-8'))
                    _data0ttreeBneg.Fill()
                    break;
                break;
    _file0.Write()
    _file0.Close()

    return

if __name__ == "__main__":
    main()
