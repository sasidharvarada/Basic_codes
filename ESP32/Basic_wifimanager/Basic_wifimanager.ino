#include <WiFiManager.h>
#include <ESP8266WiFi.h>

bool wifiConnected = false; // Declare the wifiConnected variable

void setup() {
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    Serial.begin(115200);
    WiFiManager wm;
    // wm.resetSettings();
    bool res;
    res = wm.autoConnect("AutoConnectAP","password"); // password protected ap
    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        Serial.println("Connected to Wi-Fi!");
        wifiConnected = true; // Update the wifiConnected variable
    }
}

void loop() {
    // Your loop code here
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");

    Serial.println("fuck off");
    delay(10000);

}
