#include <WiFiNINA.h>
#include <ThingSpeak.h>

#define WIFI_SSID "Your_WiFi_SSID"
#define WIFI_PASS "Your_WiFi_Password"

#define CHANNEL_ID "1424384"
#define CHANNEL_API_KEY "YGO4TGHIV7P3RAI3"

WiFiClient wifiClient;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi.");

  // Initialize ThingSpeak
  ThingSpeak.begin(wifiClient);
}

void loop() {
  // Simulate temperature and humidity data (you can use your own sensors here)
  float temperature = random(20, 30);
  float humidity = random(40, 60);

  // Send data to ThingSpeak
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  int statusThingspeak = ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY);

  Serial.print("Status Code thingspeak: ");
  Serial.println(statusThingspeak);

  if (statusThingspeak == 200) {
    Serial.println("Data sent to ThingSpeak successfully!");
  } else {
    Serial.print("Failed to send data to ThingSpeak. HTTP error code: ");
    Serial.println(statusThingspeak);
  }

  // Wait for 15 seconds before the next iteration
  delay(15000);
}
