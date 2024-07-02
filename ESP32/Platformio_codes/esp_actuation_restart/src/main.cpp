#include <WiFi.h>
#include <HTTPClient.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#define PORT 8100
#define BAUD 115200
#define NODE_ID "WM-WF-KH95-00"

// JSON document size
StaticJsonDocument<512> doc;

// AsyncWebServer instance
AsyncWebServer server(PORT);

// Flag for data reception
bool isDataReceived = false;

// Variables to hold the parsed node ID and value
String node_id;
String value;

// Function prototype for data processing
void action(const char* data_input);

// HTTPClient instance
HTTPClient http;
float temp, rH;
// WiFi credentials
const char* ssid = "esw-m19@iiith";
const char* password = "e5W-eMai@3!20hOct";

#define CSE_IP      "dev-onem2m.iiit.ac.in"
#define CSE_PORT    443
#define HTTPS       false
#define OM2M_ORGIN  "Tue_20_12_22:Tue_20_12_22"
#define OM2M_MN     "/~/in-cse/in-name/"
#define OM2M_AE     "AE-DM"
#define OM2M_DATA_CONT  "DM-KH98-60/Data"

void wifiConnect() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected to WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

void startServer() {
  // Configure server route and data handling
  server.on("/", HTTP_POST, [](AsyncWebServerRequest* request) {}, NULL, [](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
    String jsonString;
    for (size_t i = 0; i < len; i++) {
      jsonString += (char)data[i];
    }
    DeserializationError error = deserializeJson(doc, jsonString);
    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
      return;
    }
    JsonObject m2m_cin = doc["m2m:sgn"]["m2m:nev"]["m2m:rep"]["m2m:cin"];
    const char* m2m_cin_con = m2m_cin["con"];
    action(m2m_cin_con);
    request->send(200, "application/json", jsonString);
    isDataReceived = true; // Set flag after receiving data
  });

  // Start server
  server.begin();
  Serial.println("Server started");
}

// Implement the 'action' function for data processing
void action(const char* data_input) {
  // Parse the received data
  StaticJsonDocument<512> tempDoc;  // Create a temporary document for parsing
  DeserializationError error = deserializeJson(tempDoc, data_input);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  // Assuming data_input is in the format "[1718878451, 'Motor', 'DM-KH98-60' ,'1']"
  JsonArray array = tempDoc.as<JsonArray>();
  if (!array.isNull() && array.size() >= 3) {
    node_id = array[1].as<String>();
    value = array[2].as<String>();

    // Print node_id and value
    Serial.print("Node ID: ");
    Serial.println(node_id);
    Serial.print("Value: ");
    Serial.println(value);

    // Check if node_id matches NODE_ID and value is "1"
    if (node_id == NODE_ID && value == "1") {
      Serial.println("Conditions met. Restarting ESP32...");
      delay(3000);  // Wait for 3 seconds before restarting
      ESP.restart(); // Restart the ESP32
    } else {
      Serial.println("Conditions not met. No restart needed.");
    }

  } else {
    Serial.println("Invalid JSON format");
  }
}

void read_data() {
  temp = random(30, 50); // Generate random temperature between 30 and 50
  rH = random(450, 560) / 10.0; // Generate random relative humidity between 45.0 and 56.0
}
void post_data() {
  String serverURL = (HTTPS ? "https://" : "http://") + String(CSE_IP) + ":" + String(CSE_PORT) + OM2M_MN + OM2M_AE + "/" + OM2M_DATA_CONT;
  http.begin(serverURL);
  http.addHeader("X-M2M-Origin", OM2M_ORGIN);
  http.addHeader("Content-Type", "application/json;ty=4");

  // Format sensor data
  String data = "[" + String(temp) + "," + String(rH, 2) + "]";

  // Construct the request JSON
  StaticJsonDocument<256> req_doc;
  JsonObject cin_obj = req_doc.createNestedObject("m2m:cin");
  cin_obj["con"] = data;
  cin_obj["cnf"] = "text";

  // Serialize the JSON to a string
  String req_data;
  serializeJson(req_doc, req_data);

  Serial.println("Server URL: " + serverURL);
  Serial.println("Request Data: " + req_data);

  int responseCode = http.POST(req_data);
  Serial.println("HTTP Response Code: " + String(responseCode));

  if (responseCode == 400) {
    String response = http.getString();
    Serial.println("Server Response: " + response);
  }

  http.end();
}


void setup() {
  Serial.begin(BAUD);

  // Connect to WiFi
  wifiConnect();

  // Start the server
  startServer();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    wifiConnect();
  }

  if (isDataReceived) {
    isDataReceived = false;
    // Process the received data here (e.g., send to another task)
    // ...
  }
  read_data();
  post_data(); // Send sensor data to the server
  delay(10000); // Adjust delay as needed
}
