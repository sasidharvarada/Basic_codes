#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 1); // RX, TX (change these pins if needed)

void setup() {
  Serial.begin(115200); // Open serial communications
  mySerial.begin(9600); // Start the SoftwareSerial communication
  Serial.println("Ready to receive and send data");
}

void loop() {
  // Check if data is available to read from SoftwareSerial
  if (mySerial.available()) {
    char receivedChar = mySerial.read(); // Read from SoftwareSerial
    Serial.print("Received: ");
    Serial.println(receivedChar); // Print the received character
    }
    // Send a response back via SoftwareSerial
    mySerial.write("0x65"); 
    delay(1000); // Wait for a second before exiting
}
