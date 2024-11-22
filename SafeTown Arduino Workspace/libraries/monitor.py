################################################################################
# Filename: monitor.py
# Created: 10-16-24
# Modified: 11-22-24
# Author: Benjamin Kinard (SafeTown)
# Purpose: Capture the output from an RP2040 printing to the serial monitor
#   and save it to a file with the timestamp in the filename.
# Usage:
#   1. Install the "pyserial" library by running `pip install pyserial`.
#   2. Restart the IDE (if runnning in an IDE such as VSCode).
#   3. Make sure the serial monitor is not open in any Arduino IDE windows,
#      as this will prevent this script from connecting to the RP2040.
#   4. Connect the RP2040.
#   5. Run monitor.py (using the VSCode python extension is easiest).
#   6. Output data from the RP2040.
#   7. If using the data format below, then the .csv file should automatically
#      be generated at the level monitor.py was run from. Otherwise, the output
#      output from the RP2040 will simply be printed to the console.
# Data Format:
#   -----BEGIN DATA-----
#   DATA TYPE: <filename>
#   <csv data>
#   -----END DATA-----
################################################################################

import serial
import datetime

def readserial(comport, baudrate):
    ser = serial.Serial(comport, baudrate, timeout=0.1)
    fileData = ''
    addToFile = False
    filename = 'data'
    print('Ready to receive data...')

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
                print('File saved. Terminating...')
                return
            elif addToFile:
                # Add data to file output
                fileData += data + '\n'
            print(data)

if __name__ == '__main__':
    readserial('COM4', 115200)