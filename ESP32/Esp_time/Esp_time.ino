#include <ESP32Time.h>
#include <ESP8266WiFi.h>//esp8266
// #include <wifi.h>//esp32

#define MAIN_SSID "IIIT-Guest"
#define MAIN_PASS "I%GR#*S@!"

ESP32Time rtc(19800);  // offset in seconds GMT+5:30 (India Standard Time)
const char* ntpServer = "pool.ntp.org";

void setup() {
  Serial.begin(115200);
  configTime(0, 0, ntpServer);
  WiFi.begin(MAIN_SSID, MAIN_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("#");
  }

  // No need to set RTC time manually, configTime() handles NTP synchronization
  rtc.offset = 19800; // Set the RTC offset
}

void loop() {
  Serial.println(rtc.getDateTime(true));  // Print the current date and time from RTC
  Serial.println(rtc.getEpoch());         // Print the current epoch time (Unix timestamp)
  delay(1000);  // Delay 1 second
}
