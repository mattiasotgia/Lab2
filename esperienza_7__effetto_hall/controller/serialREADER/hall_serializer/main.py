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

TX3_DICT = {
    'start_remote': 'REMOTE:START?',
    'start_run_confirm': 'INO:RUN',
    'end_run_confirm': 'INO:STOP'
}

def setserial(port, baudrate = 9600):
    devSERIAL = Serial(port=port, baudrate=baudrate)
    try:
        devSERIAL.open() 
    except SerialException:
        print('Unable to establish serial comm. on port {}'.format(devSERIAL.port))
    except:
        print('Unknown error!')

def serial_write(serial: Serial, command):
    try:
        serial.write(command.encode('utf-8'))
    except:
        print('Failed command "{} to serial {serial}"'.format(command, serial=serial.port))
    time.sleep(0.1)

def main():

    output_file: str;

    return
