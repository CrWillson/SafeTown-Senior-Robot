################################################################################
# Filename: rp2040_serial_monitor.py
# Created: 10-16-24
# Modified: 11-25-24
# Author: Benjamin Kinard (SafeTown)
# Purpose: Capture the output from an RP2040 printing to the serial monitor
#   and save it to a file with the timestamp in the filename.
# Usage:
#   1. Make sure python3 is installed on the machine. If using an IDE such as
#      VSCode (recommended), install the python extension.
#   2. Install the "pyserial" library by running `pip install pyserial`.
#   3. Restart the IDE (if using one).
#   4. Make sure the serial monitor is not open in any Arduino IDE windows,
#      as this will prevent this script from connecting to the RP2040.
#   5. Connect the RP2040 to the machine.
#   6. Confirm which port the RP2040 is connected to using the Windows Device
#      Manager > Ports. If it is not using "COM4" then change line of code at
#      the bottom of this file to reflect the correct port.
#   7. Run monitor.py (press the "Run Python File" button if using the VSCode 
#      python extension).
#   8. Output data from the RP2040 (printed using Serial.println()).
#   9. If using the data format below, then the .csv file should automatically
#      be generated at the level monitor.py was run from (this may be in
#      C:\Users\<user> if you didn't open a folder in VSCode). Otherwise, the
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
    readserial('COM4', 115200) # Change the port number here if not using COM4