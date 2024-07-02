#include <ESP32Time.h>
#include <WiFi.h>

#define MAIN_SSID "IIIT-Guest"
#define MAIN_PASS "I%GR#*S@!"

ESP32Time rtc(19800);  // offset in seconds GMT+5:30 (India Standard Time)

const char* ntpServer = "pool.ntp.org";

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(MAIN_SSID, MAIN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wi-Fi connected!");

  // Configure NTP time synchronization
  configTime(0, 0, ntpServer);

  // Set the RTC time (NTP sync will happen automatically during this call)
  rtc.setTime(0); // Pass 0 to use the current time fetched from NTP server
}

void loop() {
  Serial.println(rtc.getDateTime(true));  //  (String) Sunday, January 17 2021 15:24:38
  Serial.println(rtc.getEpoch());         //  (long)    1609459200
  delay(1000);
}
