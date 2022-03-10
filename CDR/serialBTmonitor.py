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
 
#ser = serial.Serial(port=port, baudrate=9600, bytesize=8, timeout=2, parity=serial.PARITY_EVEN, rtscts=1)# stopbits=serial.STOPBITS_ONE) ##(port, 115200, timeout=10)
ser = serial.Serial(port=port, baudrate=9600, bytesize=8, timeout=10)
#ser.rtscts = 1
#ser.dsrdtr = 1

os.system('cls')

f = open('data.csv', 'a')
"""
outputCharacters = []
while 1:
    while 1:
        ch = ser.read()
        if len(ch) == 0:
            break
        outputCharacters += ch
        if outputCharacters[-len('<>'):]=='<>':
            break
    #9outputLines = ''.join(outputCharacters)

    print(outputCharacters)

"""
while True:
    x = ser.readline().decode().strip('\n')
    if x != '':
        print(x)
"""
    if ser.inWaiting() > 0:
        try:
            print(ser.readline().decode("Ascii"))
        except:
            pass
        #print(ser.readline().decode().strip('\n'))
        #f.write(ser.readline().decode().strip('\n'))
    """
		

