#include <WiFi.h>
#include <ArduinoHttpClient.h>

// Replace these with your WiFi credentials
const char* MAIN_SSID = "IIIT-Guest";
const char* MAIN_PASS = "I%GR#*S@!";

// Replace these with your OM2M platform details
const char* CSE_IP = "dev-onem2m.iiit.ac.in";
const int CSE_PORT = 443;
const bool HTTPS = false;
const char* OM2M_ORGIN = "Tue_20_12_22:Tue_20_12_22";
const char* OM2M_MN = "/~/in-cse/in-name/";
const char* OM2M_AE = "AE-WM/WM-WD";
const char* OM2M_DATA_CONT = "WM-WD-PL00-00/Data";

HttpClient http; 

void setupWiFi() {
  WiFi.begin(MAIN_SSID, MAIN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("#");
  }
  Serial.println("\nConnected to WiFi!");
}

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  setupWiFi();
}

void loop() {
  static int i = 0;
  float temp = random(27, 48);
  float rh = random(60, 85);

  // Prepare the data in JSON format
  String data = String() + "{\"temperature\":" + String(temp) + ", \"humidity\":" + String(rh) + "}";

  // Prepare the server URL
  String server = "http";
  if (HTTPS) {
    server += "s";
  }
  server += "://" + String(CSE_IP) + ":" + String(CSE_PORT) + String(OM2M_MN);

  // Create HTTP client and POST the data
  HttpClient http(wifiClient); // Pass the WiFiClient object to the HttpClient constructor
  http.begin(server + OM2M_AE + "/" + OM2M_DATA_CONT);
  http.addHeader("X-M2M-Origin", OM2M_ORGIN);
  http.addHeader("Content-Type", "application/json;ty=4");
  int code = http.POST(data);
  http.end();

  // Print response status code
  Serial.print("Status code: ");
  Serial.println(code);

  // Wait before sending data again (adjust as needed)
  delay(5000);
}
