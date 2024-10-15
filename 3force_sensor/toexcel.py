import serial
import time
import pandas as pd

# Set up the Arduino serial port and baud rate
arduino_port = '/dev/cu.usbmodem1401'  # Adjust for your port
baud_rate = 9600
timeout = 1  # Timeout in seconds

# List to hold the data
data = []


# Function to save the data to an Excel file
def save_to_excel(data_list, file_name="force_data.xlsx"):
    if data_list:
        df = pd.DataFrame(data_list, columns=["Number", "Force_sensor_1", "Force_sensor_2", "Force_sensor_3"])
        df.to_excel(file_name, index=False, engine='openpyxl')
        print(f"Data saved to {file_name}")
    else:
        print("No data to save.")


def read_from_arduino():
    try:
        # Initialize the serial connection
        ser = serial.Serial(arduino_port, baud_rate, timeout=timeout)
        time.sleep(2)  # Allow time for the connection to establish

        print("Connected to Arduino, waiting for data...")

        round_number = 1
        sensor1, sensor2, sensor3 = None, None, None

        while True:
            if ser.in_waiting > 0:
                # Read the data from Arduino
                line = ser.readline().decode('utf-8').strip()  # Read a line and remove any trailing whitespace
                print(f"Raw data from Arduino: {line}")  # Print raw data for debugging

                if "Sensor 1 Average Force" in line:
                    sensor1 = float(line.split(": ")[1].replace(" grams", ""))
                elif "Sensor 2 Average Force" in line:
                    sensor2 = float(line.split(": ")[1].replace(" grams", ""))
                elif "Sensor 3 Average Force" in line:
                    sensor3 = float(line.split(": ")[1].replace(" grams", ""))

                # When we have values for all three sensors, save them
                if sensor1 is not None and sensor2 is not None and sensor3 is not None:
                    data.append([round_number, sensor1, sensor2, sensor3])
                    print(f"Round {round_number}: Sensor1: {sensor1}, Sensor2: {sensor2}, Sensor3: {sensor3}")
                    round_number += 1
                    # Reset sensors for the next round of data
                    sensor1, sensor2, sensor3 = None, None, None

            time.sleep(0.1)  # Small delay to avoid overloading the CPU

    except serial.SerialException as e:
        print(f"Error connecting to Arduino: {e}")
    except KeyboardInterrupt:
        print("Program interrupted.")
    finally:
        # Ensure the serial connection is closed when finished
        if ser.is_open:
            ser.close()
            print("Serial connection closed.")

        # Save the collected data to an Excel file
        save_to_excel(data, file_name="force_data.xlsx")


if __name__ == "__main__":
    read_from_arduino()
