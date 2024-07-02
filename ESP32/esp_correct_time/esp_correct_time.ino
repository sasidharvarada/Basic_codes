#include <WiFi.h>
#include <time.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP32Time.h>

WiFiUDP ntpUDP;
// NTPClient timeClient(ntpUDP, "time.google.com");
NTPClient timeClient(ntpUDP, "pool.ntp.org");
ESP32Time rtc(0);

#define MIN_VALID_TIME 1672531200
#define MAX_VALID_TIME 2082844799

static uint64_t ntp_epoch_time = 0;
String ntp_date_time = "0";

#define WIFI_SSID "IIIT-Guest"
#define WIFI_PASSWORD "I%GR#*S@!"

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
      uint64_t ntp_time = timeClient.getEpochTime();
      // Serial.println(ntp_time);
      // Validate the obtained NTP time
      if (ntp_time != 0 && ntp_time >= MIN_VALID_TIME && ntp_time <= MAX_VALID_TIME) {
        // Set the RTC time using the NTP time
        rtc.setTime(ntp_time);
        ntp_epoch_time = rtc.getEpoch();
        // Serial.println(ntp_epoch_time);
        if(ntp_epoch_time < 1672000000){
          sync_time();
        }
        ntp_date_time = rtc.getTime("%A %B %d %Y %H:%M:%S");  // (String) returns time with specified format;
        Serial.print("date:");
        Serial.println(ntp_date_time);
        // Print the universal time and epoch time
          Serial.print("Universal time: " + rtc.getDateTime(true) + "\t");
          Serial.println(ntp_epoch_time, DEC); // For UTC, take timeoffset as 0
          // Serial.println("");
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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initWiFi();

}

void loop() {
  // put your main code here, to run repeatedly:
sync_time();
delay(3000);
}
