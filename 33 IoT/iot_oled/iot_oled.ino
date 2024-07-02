#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1

#define OLED_SDA 21
#define OLED_SCL 22

#define ONE_WIRE_BUS 2 // DS18B20 data pin

// Create instances for DS18B20 temperature sensor and DallasTemperature library
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Create the display object
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// TDS sensor variables
const int tdsPin = A0; // TDS sensor analog input pin

void setup() {
  Serial.begin(9600);

  delay(250); // wait for the OLED to power up
  display.begin(0x3C, true); // Address 0x3C default
  pinMode(OLED_SDA, INPUT_PULLUP); // set pin to input
  pinMode(OLED_SCL, INPUT_PULLUP); // set pin to input

  display.display();
  delay(2000);
}

void loop() {
  // Read TDS value from sensor
  int tdsValue = analogRead(tdsPin);

  // Read temperature value from DS18B20 sensor
  sensors.requestTemperatures();
  float Temp = sensors.getTempCByIndex(0);

  // Clear the display buffer.
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);

  // Display TDS value
  display.setCursor(0, 15);
  display.print("TDS Value: ");
  display.print(tdsValue); // You may need to map the tdsValue to a meaningful range

  // Display temperature value
  display.setTextSize(2);
  display.setCursor(0, 40);
  display.print("Temp: ");
  display.print(Temp);
  display.print(" C");

  // Update the OLED display
  display.display();

  // Delay before updating the display again
  delay(1000); // You can adjust the delay according to your needs
}
