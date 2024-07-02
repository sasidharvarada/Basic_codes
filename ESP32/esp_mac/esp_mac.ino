// #include <ESP8266WiFi.h>        // for esp 8266
#include <WiFi.h>                  // for esp32 

  // Wi-Fi network credentials

  // const char* ssid = "IIIT-Guest";
  // const char* password = "I%GR#*S@!";  

  // const char* ssid = "Sasi";
  // const char* password = "123456789acb"; 

  const char* ssid = "esw-m19@iiith";
  const char* password = "e5W-eMai@3!20hOct";  

  // const char* ssid = "SCRC_LAB_IOT";     
  // const char* password = "Scrciiith@123"; 


void setup() {
  Serial.begin(115200); // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }

  // Get and print MAC address
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
  
  // Print local IP address
  Serial.println('\n');
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Get and print subnet mask
  Serial.print("Subnet mask: ");
  Serial.println(WiFi.subnetMask());

  // Get and print gateway
  Serial.print("Gateway IP address: ");
  Serial.println(WiFi.gatewayIP());

  // Get and print primary DNS server
  Serial.print("Primary DNS server: ");
  Serial.println(WiFi.dnsIP());

  // Get and print secondary DNS server
  Serial.print("Secondary DNS server: ");
  Serial.println(WiFi.dnsIP(1));
}

void loop() { 
    // Get and print MAC address
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
  delay(5000);
}
