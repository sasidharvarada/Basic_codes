#include <SoftwareSerial.h>

// Pin definitions for SDS011
#define SDS011_RX_PIN 16
#define SDS011_TX_PIN 17

SoftwareSerial sds011Serial(SDS011_RX_PIN, SDS011_TX_PIN);  // RX, TX

// Buffer for reading data
uint8_t data[10];

void setup() {
  Serial.begin(115200);
  sds011Serial.begin(9600);  // SDS011 uses 9600 baud rate
  Serial.println("SDS011 ESP32 Interface");
}

void loop() {
  if (sds011Serial.available() >= 10) {
    // Read 10 bytes of data
    for (int i = 0; i < 10; i++) {
      data[i] = sds011Serial.read();
    }

    // Validate the data packet (start and end byte)
    if (data[0] == 0xAA && data[9] == 0xAB) {
      int pm25 = (data[2] + (data[3] << 8)) / 10.0;  // PM2.5
      int pm10 = (data[4] + (data[5] << 8)) / 10.0;  // PM10

      // Print the values to the Serial Monitor
      Serial.print("PM2.5: ");
      Serial.print(pm25);
      Serial.print(" µg/m³, ");
      Serial.print("PM10: ");
      Serial.print(pm10);
      Serial.println(" µg/m³");
    }
  }

  delay(1000);  // Read data every second
}