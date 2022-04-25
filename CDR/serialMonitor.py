# serial2csv.py  

# This program writes serial data from selected USB COM port to a CSV file

import serial
import os
import sys
import time
 
out = sys.stdout
 
port = input(
    '1 - COM1\n'
    '2 - COM2\n'
    '3 - COM3\n'
    '4 - COM4\n'
    '5 - COM5\n'
    '6 - COM6\n'
    'Select port: '
    )
 
port = 'COM'+port
 
ser = serial.Serial(port, 9600, timeout=10, stopbits=2) ##(port, 115200, timeout=10)
os.system('cls')

#f = open('data.csv', 'a')
 
while True:
        x = ser.readline()#.decode('utf-8', 'ignore').strip('\n')
        print(x)
        #f.write(x)
