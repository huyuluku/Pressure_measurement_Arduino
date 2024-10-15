import serial
import time
import re
import pandas as pd

# Configuration
port = '/dev/tty.usbmodem1301'  # Update this to the correct port identified from the terminal
baud_rate = 9600
num_sensors = 2
readings_per_sensor = 200  # Number of readings per sensor to record

# Initialize serial connection
ser = serial.Serial(port, baud_rate)
time.sleep(2)  # Wait for the connection to be established

# Initialize the DataFrame to store the pressure readings
column_names = [f'Sensor {i}' for i in range(num_sensors)]
pressure_data = pd.DataFrame(columns=column_names)

# Regular expression to extract pressure values from serial data
pressure_regex = re.compile(r'Sensor (\d+): ([-+]?[\d.]+) psi')

# Read pressure values from the serial output
for i in range(readings_per_sensor):
    sensor_values = [None] * num_sensors
    received_values = 0

    while received_values < num_sensors:
        line = ser.readline().decode('utf-8').strip()
        match = pressure_regex.search(line)

        if match:
            sensor_idx = int(match.group(1))
            pressure = float(match.group(2))

            if sensor_values[sensor_idx] is None:
                sensor_values[sensor_idx] = pressure
                received_values += 1

    # Convert the sensor values to a DataFrame and concatenate with the existing data
    new_data = pd.DataFrame([sensor_values], columns=column_names)
    pressure_data = pd.concat([pressure_data, new_data], ignore_index=True)

    # Print the value of i for each round
    print(i)

# Close the serial connection
ser.close()

# Save the data to an Excel file
pressure_data.to_excel('pressure_data111.xlsx', index=False)

print("Pressure data saved to 'pressure_data.xlsx'")
