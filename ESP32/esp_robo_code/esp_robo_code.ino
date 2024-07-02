#include <DFRobot_SGP40.h>

// Air quality variables
int pm2 = 0;
int pm10 = 0;
unsigned long prev_time = 0; // Added initialization

// co2, vocindex
DFRobot_SGP40 mySgp40;
uint16_t estimatedCO2; // Added semicolon at the end

//.........................................pm2,pm10.................................................//

byte command_frame[9] = {0xAA, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x67, 0xBB};
byte received_data[9];
int sum = 0;

void send_command(byte command) {
  command_frame[1] = command;
  sum = command_frame[0] + command_frame[1] + command_frame[2] + command_frame[3] + command_frame[4] + command_frame[5] + command_frame[8];
  int rem = sum % 256;
  command_frame[6] = (sum - rem) / 256;
  command_frame[7] = rem;
  delay(1000);
  Serial.write(command_frame, 9);
}

bool checksum() {
  sum = received_data[0] + received_data[1] + received_data[2] + received_data[3] + received_data[4] + received_data[5] + received_data[8];
  return sum == (received_data[6] * 256 + received_data[7]);
}

void calculate_pm() {
  pm2 = received_data[4] * 256 + received_data[5];
  delay(2000);
  pm10 = received_data[2] * 256 + received_data[3];
  Serial.print("PM2.5: ");
  Serial.println(pm2);
  Serial.print("PM10: ");
  Serial.println(pm10);
  Serial.println("----------PM updated successfully!-------------");
}

void PM_setup() {
  send_command(0x01);
}

void PM_Reading() {
  Serial.println("started pm ");
  if (millis() - prev_time > 5000) {
    send_command(0x02);
    prev_time = millis();
  }
  if (Serial.available() >= 9) { // Ensure there are enough bytes available to read
    Serial.readBytes(received_data, 9);
    if (checksum()) {
      calculate_pm();
    }
  }
}

//....................................Voc_index,co2..............................................//

struct CalibrationData {
  uint16_t vocIndex;
  uint16_t co2Concentration;
};

CalibrationData calibrationData[] = {
  {100, 400}, // Example data point: VOC index 100 corresponds to CO2 concentration 400 ppm
  // Add more calibration data points here
};

int numCalibrationPoints = sizeof(calibrationData) / sizeof(CalibrationData);

uint16_t estimateCO2(uint16_t vocIndex) {
  uint16_t minVoc = calibrationData[0].vocIndex;
  uint16_t maxVoc = calibrationData[numCalibrationPoints - 1].vocIndex;

  uint16_t minCO2 = calibrationData[0].co2Concentration;
  uint16_t maxCO2 = calibrationData[numCalibrationPoints - 1].co2Concentration;

  if (vocIndex <= minVoc) {
    return minCO2;
  } else if (vocIndex >= maxVoc) {
    return maxCO2;
  } else {
    float slope = (maxCO2 - minCO2) / (float)(maxVoc - minVoc);
    return minCO2 + (vocIndex - minVoc) * slope;
  }
}

void voc_co2_read() {
  mySgp40.begin();
  uint16_t index = mySgp40.getVoclndex(); // Declaration of index here
  Serial.print("VOC: ");
  Serial.println(index);
  estimatedCO2 = estimateCO2(index); // Use index here
  Serial.print("CO2: ");
  Serial.println(estimatedCO2);
  Serial.println("----------VOC & CO2 updated successfully!-------------");
}

void setup() {
  Serial.begin(9600);
  Serial.println("start");
  PM_setup(); // Send pm command

  delay(10000);
}

void loop() {
  Serial.println("start");
  PM_Reading(); // pm2, pm10
  voc_co2_read();
}
