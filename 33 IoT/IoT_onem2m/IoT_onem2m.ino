#include <WiFiNINA.h>
#include "OneWire.h"
#include "DallasTemperature.h"
#include <ArduinoHttpClient.h>

// WiFi credentials
const char* ssid = "myssid";  // Your WiFi network name
const char* password = "password";  // Your WiFi network password

// Sensor initialization
OneWire oneWire(14);  // OneWire data pin (adjust pin as needed)
DallasTemperature tempSensor(&oneWire);

// Sensor pins
const int tdssensorPin = 15;  // TDS sensor pin (analog)

// OneM2M connection information
const char* CSE_IP = "dev-onem2m.iiit.ac.in";  // Server IP
const int CSE_PORT = 443;  // HTTP port
const char* OM2M_ORGIN = "Tue_20_12_22:Tue_20_12_22";
const char* OM2M_MN = "/~/in-cse/in-name/";  // OneM2M main node
const char* OM2M_AE = "AE-WM/WM-WD";  // Application Entity
const char* OM2M_DATA_CONT = "WM-WD-VN00-00/Data";  // Data container

// Shared sensor data structure
struct SensorData {
    float temp;
    float tds;
    float tdsCompVoltage;
    float tdsWithoutTemp;
};

// Declare sensorData globally
SensorData sensorData;

void readSensorData(SensorData& data) {
    // Read temperature data
    tempSensor.requestTemperatures();
    float temp = tempSensor.getTempCByIndex(0);

    // Read TDS sensor value and convert to voltage
    int tdsSensorValue = analogRead(tdssensorPin);
    float voltage = tdsSensorValue * 3.3 / 1024.0;

    // Calculate temperature compensation coefficient
    float compensationCoefficient = 1.0 + 0.02 * (temp - 25.0);

    // Compensated TDS voltage and value
    float compensationVoltage = voltage / compensationCoefficient;
    float tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage 
                     - 255.86 * compensationVoltage * compensationVoltage 
                     + 857.39 * compensationVoltage) * 0.5;

    // Uncompensated TDS value
    float tdsValueWithoutTemp = (133.42 * voltage * voltage * voltage 
                                - 255.86 * voltage * voltage 
                                + 857.39 * voltage) * 0.5;

    // Update shared sensor data
    data.temp = temp;
    data.tds = tdsValue;  // With temperature compensation
    data.tdsCompVoltage = compensationVoltage;  // Compensated TDS voltage
    data.tdsWithoutTemp = tdsValueWithoutTemp;  // Without temperature compensation

    // Log sensor data
    Serial.print("Temperature: "); Serial.println(data.temp);
    Serial.print("TDS Voltage: "); Serial.println(voltage);  // Normal TDS voltage
    Serial.print("Compensated TDS Voltage: "); Serial.println(data.tdsCompVoltage);  // Compensated TDS voltage
    Serial.print("Uncompensated TDS: "); Serial.println(data.tdsWithoutTemp);  // TDS without temperature compensation
    Serial.print("Compensated TDS: "); Serial.println(data.tds);  // TDS with temperature compensation
}

void post_data(const SensorData& data) {
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HttpClient http(client, CSE_IP, CSE_PORT);

        int maxAttempts = 10;
        int statusCode = -1;

        for (int attempt = 1; attempt <= maxAttempts; ++attempt) {
            String dataString = "[" + String(data.temp) + ", " + String(data.tdsCompVoltage) + ", " + String(data.tdsWithoutTemp) + ", " + String(data.tds) + "]";
            String url = String(OM2M_MN) + OM2M_AE + "/" + OM2M_DATA_CONT;

            Serial.println("Posting to URL: http://" + String(CSE_IP) + ":" + String(CSE_PORT) + url);  // Print the URL

            http.beginRequest();
            http.post(url);
            http.sendHeader("X-M2M-Origin", OM2M_ORGIN);
            http.sendHeader("Content-Type", "application/json;ty=4");

            // String req_data = "{\"m2m:cin\": {\"con\": \"" + dataString + "\", \"rn\": \"cin_" + String(millis()) + "\"}}";
              String req_data = String() + "{\"m2m:cin\": {"
                    + "\"con\": \"" + dataString + "\","
                    + "\"cnf\": \"text\""
                    + "}}";
            http.sendHeader("Content-Length", req_data.length());
            http.beginBody();
            http.print(req_data);
            Serial.println("Request Data: " + req_data);
            http.endRequest();

            statusCode = http.responseStatusCode();
            Serial.println("HTTP Response Code: " + String(statusCode));

            if (statusCode == 201) {  // If successful posting
                break;
            }

            delay(2000);  // Wait 2 seconds before retrying
        }

        if (statusCode != 201) {  // If all attempts fail
            Serial.println("All attempts to post data failed. Resetting...");
            NVIC_SystemReset();  // Restart the Arduino
        }
        // Close the connection to free up sockets
        http.stop();
    }
}

void setup() {
    Serial.begin(115200);
    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Connecting to WiFi...");
        delay(500);  // Retry every 500 milliseconds
    }
    Serial.println("Connected to WiFi!");

    // Initialize sensor
    tempSensor.begin();
}

void loop() {
    // Read sensor data
    readSensorData(sensorData);
    // Post data to OneM2M
    post_data(sensorData);
    Serial.println("Waiting for 1 minute before repeating...");
    delay(1000);  // Wait for 1 minute before repeating
}
