#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_ADDRESS 0x27 // Change this to your LCD's I2C address if different
#define LCD_COLS 16
#define LCD_ROWS 2

// Initialize the LiquidCrystal_I2C object with the I2C address, number of columns, and number of rows of your LCD
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

void setup() {
  Serial.begin(9600); // Start serial communication for debugging
  Wire.begin();       // Start the I2C communication

  lcd.init();         // Initialize the LCD
  lcd.backlight();    // Turn on the backlight
  lcd.clear();        // Clear any existing characters on the LCD

  // Set up initial display
  lcd.setCursor(0, 0); // Set cursor to first column, first row
  lcd.print("Hello"); // Display the text for pressure
  
  lcd.setCursor(0, 1); // Set cursor to first column, second row
  lcd.print("World"); // Display the static pressure value
}

void loop() {
  // Since the display is static, no need to update in the loop unless you want to add dynamic content
  delay(1000); // Just a placeholder to keep the loop cycling (optional for static display)
}
