#define DO_PIN 15  // ESP32's digital pin GPIO13 connected to DO pin of the LDR module

int luxValue = 0;  // Global variable to store the estimated lux value

void setup() {
  // initialize serial communication
  Serial.begin(9600);
  // initialize the ESP32's pin as an input
  pinMode(DO_PIN, INPUT);
}

void loop() {
  int lightState = digitalRead(DO_PIN);

  if (lightState == HIGH) {
    // Set a higher value for lux when it's dark (sensor reading HIGH)
    luxValue = 1000; // Adjust this value based on your estimation
  } else {
    // Set a lower value for lux when it's light (sensor reading LOW)
    luxValue = 100; // Adjust this value based on your estimation
  }

  Serial.print("Estimated Lux value: ");
  Serial.println(luxValue);

  delay(1000);  // Delay for stability, adjust as needed
}
