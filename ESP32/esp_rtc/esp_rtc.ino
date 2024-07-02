#include <Wire.h>      // Include the Wire library for I2C communication
#include <RTClib.h>    // Include the RTC library

RTC_DS3231 rtc;        // Create an RTC object

void setup() {
  Serial.begin(115200); // Initialize serial communication with the computer
  while (!Serial);      // Wait for serial port to connect (for boards that require it)
  
  Wire.begin();        // Start the I2C communication
  rtc.begin();         // Initialize the RTC

  // Uncomment the following line and set the time only once
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  unsigned long currentMillis = millis(); // Get the current milliseconds since the board started
  DateTime now = rtc.now();    // Read the current time from RTC

  // Access individual time components
  int year = now.year();
  int month = now.month();
  int day = now.day();
  int hour = now.hour();
  int minute = now.minute();
  int second = now.second();

  // Print the timestamp, date, and time to the serial monitor
  Serial.print("[");
  Serial.print(currentMillis);
  Serial.print(" ms] ");
  Serial.print("Current Time: ");
  Serial.print(year);
  Serial.print("-");
  print2digits(month);
  Serial.print("-");
  print2digits(day);
  Serial.print(" ");
  print2digits(hour);
  Serial.print(":");
  print2digits(minute);
  Serial.print(":");
  print2digits(second);
  Serial.println();

  // Use the time for your project as needed
  // ...

  delay(1000);    // Wait for a second
}

// Helper function to print two digits for time formatting
void print2digits(int number) {
  if (number < 10) {
    Serial.print("0");
  }
  Serial.print(number);
}
