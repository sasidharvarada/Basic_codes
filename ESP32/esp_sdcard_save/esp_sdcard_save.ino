#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <SD.h>
#include "time.h"

const char* WIFI_SSID = "SCRC_LAB_IOT";
const char* WIFI_PASSWORD = "Scrciiith@123";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println();
  Serial.println("Connected to WiFi");
}

void time_setup() {
  timeClient.setPoolServerName("pool.ntp.org"); // Set NTP server
  timeClient.begin();
  timeClient.setTimeOffset(0); // Set your time offset here (in seconds)
}

void time_loop() {
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm* timeinfo;
  time_t rawtime = (time_t)epochTime;
  timeinfo = localtime(&rawtime);
  char timestamp[20];
  strftime(timestamp, sizeof(timestamp), "%Y/%m/%d %H:%M:%S", timeinfo);
  Serial.println(timestamp);

  File rawDataFile = SD.open("/raw_data.csv", FILE_APPEND);
  if (rawDataFile) {
    rawDataFile.print(timestamp);
    rawDataFile.print(",");
    rawDataFile.println();
    rawDataFile.close();
    Serial.println("Time updated successfully!");
  } else {
    Serial.println("Error updating time");
    esp_restart();
  }
}

void SD_setup() {
  if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }

  if (!SD.exists("/raw_data.csv")) {
    File rawDataFile = SD.open("/raw_data.csv", FILE_WRITE);
    if (rawDataFile) {
      rawDataFile.println("timeStamp");
      rawDataFile.close();
      Serial.println("Raw data CSV file created successfully!");
    } else {
      Serial.println("Error creating raw data CSV file");
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting ESP32...");
  initWiFi();
  time_setup();
  SD_setup();
}

void loop() {
  time_loop();
  delay(6000); // Save timestamp every 6 seconds
}
