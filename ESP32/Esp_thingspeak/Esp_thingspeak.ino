#include <WiFi.h>
#include <ThingSpeak.h>

#define WIFI_SSID "SCRC_LAB_IOT"
#define WIFI_PASSWORD "Scrciiith@123"


const unsigned long channelID = 1424384;
const char* apiKey = "YGO4TGHIV7P3RAI3";

WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  ThingSpeak.begin(client);
}

void loop() {
  // Read your sensor data here and store it in variables
  float temperature = 25.5; // Replace with your actual temperature value
  float humidity = 50.0; // Replace with your actual humidity value

  // Send data to ThingSpeak
  ThingSpeak.setField(1, temperature); // Field1 for temperature
  ThingSpeak.setField(2, humidity);    // Field2 for humidity

  int httpCode = ThingSpeak.writeFields(channelID, apiKey);

  if (httpCode == 200) {
    Serial.println("Data sent to ThingSpeak successfully!");
  } else {
    Serial.print("Error sending data to ThingSpeak. HTTP error code: ");
    Serial.println(httpCode);
  }

  delay(5000); // Send data every 5 seconds (adjust as per your requirement)
}
