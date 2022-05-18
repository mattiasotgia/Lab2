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

def readbatch(_data0ttreeB, _file00read, M = 12, N = 100):
    _file00read.readline() # *BPOS/*BNEG
    for i in range(M):
        _file00read.readline() # *CYCLESTART
        _file00read.readline() # I:0

        root.Event.ID = 1
        root.Event.RUN = i
        root.Event.BUFSIZE = N

        for j in range(N):
            V_out_hi = float(_file00read.readline())
            V_gen_hi = float(_file00read.readline())
            V_out_lo = float(_file00read.readline())
            V_gen_lo = float(_file00read.readline())
            # TODO: write data to array
        # TODO: compute mean and avg
        _file00read.readline() # CYCLEEND*
        _data0ttreeB.Fill()

def main():
    
    Bpol = ['Bpos', 'Bneg', 'B?']
    today: str = datetime.today().strftime('%Y%m%d_%H.%M.%S.%f');
    output_file: str = 'dataRUN_{}_HALL.root'.format(today)

    
    _file0 = root.TFile(join(BASE_DATA_PATH, output_file), 'RECREATE')

    

    root.gInterpreter.ProcessLine('''
        struct Event {
            UInt_t ID;
            UInt_t RUN;
            UInt_t BUFSIZE;
            Double_t V_gen_lo[100];
            Double_t V_gen_hi[100];
            Double_t V_out_lo[100];
            Double_t V_out_hi[100];
            // TODO: add variables for mean, std, end miscelleneous
        };
    ''')

    # devSERIAL = setserial(port='/dev/tty.usbmodem141401')

    M = 12
    N = 100
    
    _file00read = open(join(BASE_DATA_PATH, 'data_dump_CHEPALLE.txt'))

    datatypes = 'ID:RUN:BUFSIZE/i:V_gen_lo[100]:V_gen_hi[100]:V_out_lo[100]:V_out_hi[100]/D'
    
    _data0ttreeBpos = root.TTree('data_BPOS', 'RUN data E7/hall: positive B')
    _data0ttreeBpos.Branch('event', root.Event(), datatypes)

    _data0ttreeBneg = root.TTree('data_BNEG', 'RUN data E7/hall: negative B')
    _data0ttreeBneg.Branch('event', root.Event(), datatypes)

    while(True):
        print(_file00read.readline() == '*RUN')
        print('{} read\n',format(_file00read.name))
        readbatch(_data0ttreeBpos, _file00read)
        _file00read.readline()
        readbatch(_data0ttreeBneg, _file00read)

        break

    
    '''
        while(True):
            if(devSERIAL.readline().decode('utf-8') == '*RUN\n'):
                print('serial comm. to START data aquisition\n')
                line = devSERIAL.readline().decode('utf-8')
                if(line == '*BPOS'):
                    _data0ttreeBpos = root.TTree('data_BPOS', 'RUN data E7/hall: positive B')
                    _data0ttreeBpos.Branch('event', root.Event(), 'ID:RUN/i:V_gen_lo[100]:V_gen_hi[100]:V_out_lo[100]:V_out_hi[100]/D')
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
                    _data0ttreeBneg.Branch('event', root.Event(), 'ID:RUN/i:V_gen_lo[100]:V_gen_hi[100]:V_out_lo[100]:V_out_hi[100]/D')
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
                        break
                    break
    '''

    _file0.Write()
    _file0.Close()
    
    return

if __name__ == "__main__":
    main()
