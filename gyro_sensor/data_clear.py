import openpyxl

# Load the existing Excel file
file_path = 'gyro_data_1_10psi_weak_pattern.xlsx'
wb = openpyxl.load_workbook(file_path)
ws = wb.active

# Iterate over the rows in the Sensor_1_Roll column
for row in ws.iter_rows(min_row=2, min_col=3, max_col=3):  # Assuming Sensor_1_Roll is in the 3rd column (C)
    for cell in row:
        if cell.value is not None:
            # Convert the cell value to string, remove commas, and convert back to float
            cleaned_value = str(cell.value).replace(',', '')
            try:
                cell.value = float(cleaned_value)
            except ValueError:
                cell.value = cleaned_value  # In case conversion to float fails, keep it as string

# Save the updated Excel file
wb.save('gyro_data_cleaned.xlsx')
print("Commas removed from Sensor_1_Roll and saved to 'gyro_data_cleaned.xlsx'")
