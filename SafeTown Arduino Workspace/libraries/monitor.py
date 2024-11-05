################################################################################
# Filename: monitor.py
# Created: 10-16-24
# Author: Benjamin Kinard
# Purpose: Capture the output from the robot printing to the serial monitor
#   and save it to a file with the timestamp in the filename.
################################################################################

import serial
import datetime

def readserial(comport, baudrate):
    ser = serial.Serial(comport, baudrate, timeout=0.1)
    fileData = ''
    addToFile = False
    filename = 'data'
    
    while True:
        data = ser.readline().decode().strip()
        if data:
            if data == '-----BEGIN DATA-----':
                # Begin reading input for file output
                addToFile = True
            elif 'DATA TYPE: ' in data:
                # Get filename for data
                filename = str(data[11:])
            elif data == '-----END DATA-----':
                # Stop reading input and save to file
                now = str(datetime.datetime.now()).split(' ')
                filename += '_' + now[0] + '_' + now[1].split('.')[0].replace(':', '-') + '.csv'
                f = open(filename, 'w')
                f.write(fileData[:-1])
                f.close()
                addToFile = False
                fileData = ''
            elif addToFile:
                # Add data to file output
                fileData += data + '\n'
            print(data)

if __name__ == '__main__':
    readserial('COM4', 115200)