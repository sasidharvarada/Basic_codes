#include <WiFi.h>
#include "time.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "SD.h"
#include <HTTPClient.h>
#include <ThingSpeak.h>
#include <ESP32Time.h>

//--------------------------------------WiFi & Time----------------------------------------------------


#define WIFI_SSID "IIIT-Guest"
#define WIFI_PASSWORD "I%GR#*S@!"

#define CHANNEL_ID 2225888
#define CHANNEL_API_KEY "8GYK8983JP0QD8YR"

unsigned long previousMillis = 0;
unsigned long interval = 30000; // Reconnection interval in milliseconds
unsigned long delayInterval = 1000; 

WiFiClient client;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
ESP32Time rtc(0);

#define MIN_VALID_TIME 1672531200
#define MAX_VALID_TIME 2082844799

static uint64_t ntp_epoch_time = 0;
String ntp_date_time = "0";

void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi");
  unsigned long startTime = millis(); // Get the current time in milliseconds

  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 30000) {
    Serial.print('.');
    delay(1000);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());    
    Serial.print("Mac Address :");
    Serial.println(WiFi.macAddress());
    long rssi = WiFi.RSSI();
    Serial.print("Signal strength (RSSI): ");
    Serial.print(rssi);
  } else {
    Serial.print("Mac Address :");
    Serial.println(WiFi.macAddress());
    Serial.println("\nFailed to connect to WiFi within 30 seconds. Restarting...");
    ESP.restart(); // Restart the device
  }
}

void sync_time() {
  static bool first_time = true;
  int num_tries = 5;  // Number of attempts to get valid NTP time

  // Check if the ESP32 is connected to WiFi
  if (WiFi.status() == WL_CONNECTED) {
    if (first_time == true) {
      // Initialize NTPClient only once
      timeClient.begin();
      timeClient.setTimeOffset(0);
      timeClient.update();
      first_time = false;
    }
    // Configure the time zone (IST: Indian Standard Time, UTC+5:30)
    configTime(5 * 3600, 30 * 60, "pool.ntp.org");
    
    while (num_tries > 0) {
      // Get the obtained NTP time
      uint64_t ntp_time = timeClient.getEpochTime();  // Apply local offset
     
      // Validate the obtained NTP time
      if (ntp_time != 0 && ntp_time >= MIN_VALID_TIME && ntp_time <= MAX_VALID_TIME) {
        // Set the RTC time using the adjusted NTP time
        rtc.setTime(ntp_time);
        ntp_epoch_time = rtc.getEpoch();
        if(ntp_epoch_time < 1672000000){
          sync_time();
        }

        ntp_date_time = rtc.getTime("%A %B %d %Y %H:%M:%S");  // (String) returns time with specified format;
        // Open the calibrated data file for appending
        File calibratedDataFile = SD.open("/DATA_Node3/calibrated_data.csv", FILE_APPEND);
        if (calibratedDataFile) {
          calibratedDataFile.print(ntp_date_time);
          calibratedDataFile.print(",");
          Serial.println("Calibrated timestamp data updated successfully!");
        } else {
          Serial.println("Error updating calibrated CO2 data");
        }

        // Update the last_update time
        return;  // Exit the function after successful synchronization
      } else {
        // Invalid NTP time, reduce the number of remaining attempts
        Serial.println("Invalid NTP time, retrying...");
        num_tries--;
        delay(1000);
        Serial.println("All attempts failed, performing restarting time function ...");
        ESP.restart();   
      }
    }
  }
}

//---------------------------------------------------SD-Card-------------------------------------------------------------------------
//File rawDataFile;
//File calibratedDataFile;

void SD_setup() {
  if (!SD.begin()) {
    Serial.println();
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

      if (!SD.exists("/DATA_Node3/raw_data.csv")) {
       File rawDataFile = SD.open("/DATA_Node3/raw_data.csv", FILE_WRITE);
        if (rawDataFile) {
      rawDataFile.println("timeStamp,PM2.5,PM10,temp,rh,co2,raw_voc,voc_index");
      rawDataFile.close();
      Serial.println("Raw data CSV file created successfully!");
    } else {
      Serial.println("Error creating raw data CSV file");
    }
  } else {
    Serial.println("Raw data file exists, Appending to existing file");
  }

  if (!SD.exists("/DATA_Node3/calibrated_data.csv")) {
    File calibratedDataFile = SD.open("/DATA_Node3/calibrated_data.csv", FILE_WRITE);
    if (calibratedDataFile) {
      calibratedDataFile.println("timeStamp,C_PM2.5,C_PM10,C_temp,C_rh,C_co2,raw_voc,voc_index");
      calibratedDataFile.close();
      Serial.println("Calibrated data CSV file created successfully!");
    } else {
      Serial.println("Error creating calibrated data CSV file");
    }
  } else {
    Serial.println("Calibrated data file exists, Appending to existing file");
  }
}

void SD_final() {
  File rawDataFile = SD.open("/DATA_Node3/raw_data.csv", FILE_APPEND);
  if (rawDataFile) {
    rawDataFile.println("");
    Serial.println("----------updated successfully!-------------");
  } else {
    Serial.println("--------------Error updating ----------------");
  }
  File calibratedDataFile = SD.open("/DATA_Node3/calibrated_data.csv", FILE_APPEND);
  if (calibratedDataFile) {
    calibratedDataFile.println("");
    Serial.println("----------updated successfully!-------------");
  } else {
    Serial.println("--------------Error updating ----------------");
  }
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}


void thingspeak_post(){
    // Write to the first channel (non-calibrated)
  ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY);

  // Set ThingSpeak fields for the second channel (calibrated values)
  ThingSpeak.setField(1, ntp_epoch_time);  
  // Write to the second channel (calibrated)
  ThingSpeak.writeFields(CALIBRATED_CHANNEL_ID, CALIBRATED_CHANNEL_API_KEY);
}


//------------------------------------------------------------------------------------------------------------

void setup() {
  
  Serial.begin(9600);
  Serial.println("Starting ESP32...");
  initWiFi();
  SD_setup();
  ThingSpeak.begin(client);
  delay(30000);
  SD_setup();
}

void loop() {
unsigned long currentMillis = millis();
  
  // If WiFi is down, try reconnecting every CHECK_WIFI_TIME milliseconds
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval)) {
    Serial.println("Reconnecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(delayInterval);
    }
    Serial.println(" WiFi reconnected");
    previousMillis = currentMillis;
  }

  sync_time();
  SD_final();
  thingspeak_post();
  delay(30000);
}
