// #include <BluetoothSerial.h> // Include the BluetoothSerial library

// BluetoothSerial SerialBT; // Define the BluetoothSerial object

// void setup() {
//   Serial.begin(115200); // Initialize serial communication
//   SerialBT.begin("ESP32_BT_Sender"); // Initialize Bluetooth
// }

// void loop() {
//   if (Serial.available() > 0) {
//     String command = Serial.readStringUntil('\n');
//     Serial.println("Sending command: " + command);

//     // Send the command via Bluetooth
//     if (SerialBT.available()) {
//       SerialBT.println(command); // Sending the command over Bluetooth
//     }
//   }
// }



#include <BluetoothSerial.h> // Include the BluetoothSerial library

BluetoothSerial SerialBT; // Define the BluetoothSerial object

void setup() {
  Serial.begin(115200); // Initialize serial communication
  SerialBT.begin("ESP32_BT_Receiver"); // Initialize Bluetooth with a unique name
}

void loop() {
  if (SerialBT.available()) {
    String receivedData = SerialBT.readStringUntil('\n');
    if (receivedData.length() > 0) {
      Serial.println("Received: " + receivedData);
      // Add your logic based on the received data here
    }
  }
}
