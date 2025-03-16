import serial

# Configure the serial port
SERIAL_PORT = "COM3"  # Change to your port (e.g., "/dev/ttyUSB0" on Linux)
BAUD_RATE = 115200
OUTPUT_FILE = "small_image_dump.bin"

def dump_serial_to_file():
    with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as ser, open(OUTPUT_FILE, "wb") as f:
        print(f"Listening on {SERIAL_PORT} at {BAUD_RATE} baud...")
        try:
            while True:
                line = ser.readline()
                if line:
                    f.write(line.strip() + b'\n')  # Write to file and add a newline
                    print(line.decode('utf-8').strip())
        except KeyboardInterrupt:
            print("\nStopped by user.")
        finally:
            print(f"Data saved to {OUTPUT_FILE}")

if __name__ == "__main__":
    dump_serial_to_file()
