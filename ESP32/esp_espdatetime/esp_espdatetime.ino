#include <WiFi.h>
#include <ESPDateTime.h>

#define MAIN_SSID "IIIT-Guest"
#define MAIN_PASS "I%GR#*S@!"

void setupDateTime() {

  Serial.begin(115200);
  configTime(0, 0, ntpServer);
  WiFi.begin(MAIN_SSID, MAIN_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("#");
  }

  DateTime.setTimeZone("IST-5:30");
  // this method config ntp and wait for time sync
  // default timeout is 10 seconds
  DateTime.begin(/* timeout param */);
  if (!DateTime.isTimeValid()) {
    Serial.println("Failed to get time from server.");
  }
}

void loop{
// alias for getTime()
time_t  DateTime.now()
// get current timestap in seconds
time_t  DateTime.getTime()
// get current timezone
char*     DateTime.getTimeZone()
// get formatted string of time
String  DateTime.toString()
// get formatted string of utc time
String  DateTime.toUTCString()
// format local time to string, using strftime
// http://www.cplusplus.com/reference/ctime/strftime/
String  DateTime.format(const char* fmt);
// format utc time to string, using strftime
// http://www.cplusplus.com/reference/ctime/strftime/
String  DateTime.formatUTC(const char* fmt);
}