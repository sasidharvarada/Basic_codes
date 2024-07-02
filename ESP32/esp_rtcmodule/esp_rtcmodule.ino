#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC; // Setup an instance of DS1307 naming it RTC

void setup() {
  Serial.begin(9600); // Set serial port speed
  Wire.begin(); // Start the I2C
  RTC.begin();  // Init RTC
  RTC.adjust(DateTime(__DATE__, __TIME__)); // Time and date is expanded to date and time on your computer at compile time
  Serial.println("Time and date set");
}

void loop() {

  DateTime now = RTC.now();

  unsigned long epochTimestamp = now.unixtime(); // Get the epoch timestamp
  Serial.print("Epoch Timestamp: ");
  Serial.println(epochTimestamp);

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(5000);
}
