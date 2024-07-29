#include <WiFiNINA.h>
#include <ArduinoJson.h>

#define BAUD 115200
#define PORT 8100

const char *ssid = "esw-m19@iiith";
const char *password = "e5W-eMai@3!20hOct";

WiFiServer server(PORT);
WiFiClient client;

void connectWiFi() {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

String readRequest(WiFiClient client) {
    String request = "";
    while (client.available()) {
        char c = client.read();
        request += c;
    }
    return request;
}

void handleRequest() {
    String request = readRequest(client);

    // Parse HTTP request
    if (request.indexOf("POST") >= 0) {
        int contentIndex = request.indexOf("\r\n\r\n");
        if (contentIndex >= 0) {
            String content = request.substring(contentIndex + 4);
            Serial.println("Received POST request");
            Serial.println("Content: " + content);

            // Deserialize JSON
            StaticJsonDocument<512> doc;
            DeserializationError error = deserializeJson(doc, content);
            if (error) {
                Serial.print("deserializeJson() failed: ");
                Serial.println(error.c_str());
                client.print("HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\nInvalid JSON");
                return;
            }

            // Process the JSON content as needed
            // Example: const char* value = doc["key"];

            client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"status\":\"success\"}");
        } else {
            client.print("HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\nBad Request");
        }
    } else {
        client.print("HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/plain\r\n\r\nMethod Not Allowed");
    }
}

void setup() {
    Serial.begin(BAUD);
    delay(1000);
    connectWiFi();

    server.begin();
    Serial.print("Server started on port ");
    Serial.println(PORT);
}

void loop() {
    client = server.available();
    if (client) {
        if (client.connected()) {
            handleRequest();
        }
        client.stop();
    }
}