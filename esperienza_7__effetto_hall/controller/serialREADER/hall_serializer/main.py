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


def readbatch(_data0ttreeB, _file00read, evt, M = 12, N = 100):
    _file00read.readline() # *BPOS/*BNEG
    for i in range(M):
        _file00read.readline() # *CYCLESTART
        _file00read.readline() # I:0

        evt.ID = 1
        evt.RUN = i+1
        evt.BUFSIZE = N

        for j in range(N):
            evt.V_out_hi[j] = float(_file00read.readline())
            evt.V_gen_hi[j] = float(_file00read.readline())
            evt.V_out_lo[j] = float(_file00read.readline())
            evt.V_gen_lo[j] = float(_file00read.readline())
            # TODO: write data to array
        _data0ttreeB.Fill()
        # TODO: compute mean and avg
        _file00read.readline() # CYCLEEND*
    _data0ttreeB.Print()
    _data0ttreeB.Write()
    

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
        };
    ''')

    # devSERIAL = setserial(port='/dev/tty.usbmodem141401')

    M = 12
    N = 100
    
    _file00read = open(join(BASE_DATA_PATH, 'data_dump_CHEPALLE.txt'))
    
    Event = root.Event()

    # datatypes = 'ID:RUN:BUFSIZE/i:V_gen_lo[100]:V_gen_hi[100]:V_out_lo[100]:V_out_hi[100]/D'
    
    _data0ttreeBpos = root.TTree('data_BPOS', 'RUN data E7/hall: positive B')
    _data0ttreeBpos.Branch('ID', root.addressof(Event, 'ID'), 'ID/i')
    _data0ttreeBpos.Branch('RUN', root.addressof(Event, 'RUN'), 'RUN/i')
    _data0ttreeBpos.Branch('BUFSIZE', root.addressof(Event, 'BUFSIZE'), 'BUFSIZE/i')
    _data0ttreeBpos.Branch('V_gen_lo', root.addressof(Event, 'V_gen_lo'), 'V_gen_lo[100]/D')
    _data0ttreeBpos.Branch('V_gen_hi', root.addressof(Event, 'V_gen_hi'), 'V_gen_hi[100]/D')
    _data0ttreeBpos.Branch('V_out_lo', root.addressof(Event, 'V_out_lo'), 'V_out_lo[100]/D')
    _data0ttreeBpos.Branch('V_out_hi', root.addressof(Event, 'V_out_hi'), 'V_out_hi[100]/D')

    _data0ttreeBneg = root.TTree('data_BNEG', 'RUN data E7/hall: negative B')
    _data0ttreeBneg.Branch('ID', root.addressof(Event, 'ID'), 'ID/i')
    _data0ttreeBneg.Branch('RUN', root.addressof(Event, 'RUN'), 'RUN/i')
    _data0ttreeBneg.Branch('BUFSIZE', root.addressof(Event, 'BUFSIZE'), 'BUFSIZE/i')
    _data0ttreeBneg.Branch('V_gen_lo', root.addressof(Event, 'V_gen_lo'), 'V_gen_lo[100]/D')
    _data0ttreeBneg.Branch('V_gen_hi', root.addressof(Event, 'V_gen_hi'), 'V_gen_hi[100]/D')
    _data0ttreeBneg.Branch('V_out_lo', root.addressof(Event, 'V_out_lo'), 'V_out_lo[100]/D')
    _data0ttreeBneg.Branch('V_out_hi', root.addressof(Event, 'V_out_hi'), 'V_out_hi[100]/D')

    while(True):
        print(_file00read.readline() == '*RUN\n')
        print('{} read\n'.format(_file00read.name))
        readbatch(_data0ttreeBpos, _file00read, Event)
        _file00read.readline()
        readbatch(_data0ttreeBneg, _file00read, Event)

        break

    _file0.Write()
    _file0.Close()
    
    return

if __name__ == "__main__":
    main()
