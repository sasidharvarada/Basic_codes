//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` and enable it
#endif

BluetoothSerial SerialBT;

const char* address = "68:27:19:A8:11:92"; // Bluetooth address in string format

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); // Bluetooth device name
  Serial.println("The device has started, now you can pair it with Bluetooth!");
}

void connectBT() {
  SerialBT.connect(address);
}

void loop() {
  if (!SerialBT.connected()) {
    Serial.println("Unable to connect");
    connectBT();
  } else {
    Serial.println("Connected");
  }
  delay(1000);
}
