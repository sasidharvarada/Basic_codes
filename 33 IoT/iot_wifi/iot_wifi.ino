#include <SPI.h>
#include <WiFiNINA.h>

// char ssid[] = "SCRC_LAB_IOT";      // Replace with your Wi-Fi network name (SSID)
// char password[] = "Scrciiith@123";  // Replace with your Wi-Fi network password

// char ssid[] = "TP-Link_0D7C";  // your network SSID (name) FACULTY MESS
// char password[] = "70288247";   // your network password (use for WPA, or use as key for WEP)

char ssid[] = "myssid";  // your network SSID (name) FACULTY MESS
char password[] = "password";   // your network password (use for WPA, or use as key for WEP)

// char ssid[] = "TP-Link_208C";  // your network SSID (name) PARIJAT OHT
// char password[] = "88257594";   // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;
WiFiClient client;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect
  }

  // Check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  // Attempt to connect to Wi-Fi network
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, password);
    delay(5000); // Delay for connection attempt
  }

  // Print connection details
  Serial.println("Connected to Wi-Fi");
  printConnectionDetails();
}

void loop() {
  // Your code here

}

void printConnectionDetails() {
  // Print the board's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Print the MAC address
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC Address: ");
  for (int i = 0; i < 6; i++) {
    Serial.print(mac[i], HEX);
    if (i < 5) {
      Serial.print(":");
    }
  }
  Serial.println();
}
