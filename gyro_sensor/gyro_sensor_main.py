import serial
import time
from openpyxl import Workbook

# Initialize the serial port
ser = serial.Serial('/dev/cu.usbmodem1401', 115200)  # Replace with your serial port address

# Specify how long to measure (in seconds)
measure_time = 400  # Change this value as needed

# Create a new Excel workbook and sheet
wb = Workbook()
ws = wb.active
ws.title = "Sensor Data"
# Write the header
ws.append(['Sensor_1_Yaw', 'Sensor_1_Pitch', 'Sensor_1_Roll', 'Sensor_2_Yaw', 'Sensor_2_Pitch', 'Sensor_2_Roll'])

start_time = time.time()
discard_lines = 10  # Number of initial lines to discard
discarded = 0
data_count = 0  # Counter for the number of data entries written

while True:
    try:
        # Check if the measure_time has elapsed
        if time.time() - start_time > measure_time or data_count >= 300:
            print("\nMeasurement time has elapsed or reached 300 entries. Terminating the script.")
            break

        # Read a line from the serial port
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            print(f"Raw line: {line}")  # Debug: print the raw line read from the serial port

            # Discard the initial unstable lines
            if discarded < discard_lines:
                discarded += 1
                continue

            # Split the line into parts based on the prefix and separator
            parts = line.split("YPR #2: ")
            ypr1_part = parts[0].replace("YPR #1: ", "").strip()
            ypr2_part = parts[1].strip() if len(parts) > 1 else ""

            # Ensure the parts are split correctly and clean any extra commas
            ypr1_values = ypr1_part.split(", ")
            ypr2_values = ypr2_part.split(", ") if ypr2_part else []

            # Remove any trailing commas or extra whitespace from each value
            ypr1_values = [val.rstrip(',').strip() for val in ypr1_values]
            ypr2_values = [val.rstrip(',').strip() for val in ypr2_values]

            # Combine the cleaned values
            values = ypr1_values + ypr2_values

            print(f"Processed values: {values}")  # Debug: print the processed values

            # Check if we have the correct number of values
            if len(values) == 6:
                # Increment the data count
                data_count += 1

                # Write the values to the Excel sheet
                ws.append(values)
                print(f"Progress: {data_count}/300", end='\r')  # Display progress in the terminal
    except KeyboardInterrupt:
        print("\nTerminating the script.")
        break
    except Exception as e:
        print(f"Error: {e}")
        continue

# Save the workbook
wb.save("weakest pattern data 1.xlsx")
print("\nData saved to sensor_data.xlsx")
