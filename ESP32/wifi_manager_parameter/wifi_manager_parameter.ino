#include <WiFiManager.h>

int custom_integer = 0; // Variable to store the custom integer value
bool wifiConnected = false;
bool wifiSetDone = false; // Flag to track if wifi_set() has been done

void setup() {
  // Setup Serial Monitor
  Serial.begin(115200);
}

void wifi_set() {
  // Create WiFiManager object
  WiFiManager wfm;

  // Supress Debug information
  wfm.setDebugOutput(true);

  // Remove any previous network settings
  // wfm.resetSettings();

  // Define an integer input field
  WiFiManagerParameter custom_integer_input("my_integer", "Enter an integer value", "0", 5); // 5 digits maximum

  // Add custom parameter
  wfm.addParameter(&custom_integer_input);

  if (!wifiSetDone) {
    if (!wfm.autoConnect("ESP32TEST_AP", "password")) {
      // Did not connect, print error message
      Serial.println("Failed to connect and hit timeout");

      // Reset and try again
      // ESP.restart();
      delay(1000);
    } else {
      // Connected!
      Serial.println("WiFi connected");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      wifiConnected = true; // Update the wifiConnected variable

      // Print custom integer value to serial monitor
      custom_integer = atoi(custom_integer_input.getValue());
      Serial.print("Custom integer value: ");
      Serial.println(custom_integer);

      wifiSetDone = true; // Mark wifi_set() as done
    }
  }
}

void loop() {
  // Your loop code 

  wifi_set();

  delay(10000);
  Serial.println(custom_integer);
}
