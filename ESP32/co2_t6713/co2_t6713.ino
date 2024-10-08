// #include <Wire.h> // Although Wire.h is part of the Ardunio GUI
// // library, this statement is still necessary
// #define ADDR_6713 0x15 // default I2C slave address
// int data [4];
// int CO2ppmValue;
// // This is the default address of the CO2 sensor, 7bits shifted
// // left.
// void setup() {
//  Wire.begin ();
//  Serial.begin(9600);
//  Serial.println("Application Note AN161_ardunio_T6713_I2C");
// }
// //////////////////////////////////////////////////////////////////
// // /
// // Function : int readCO2()
// // Returns : CO2 Value upon success, 0 upon checksum failure
// // Assumes : - Wire library has been imported successfully.
// // - LED is connected to IO pin 13
// // - CO2 sensor address is defined in co2_addr
// //////////////////////////////////////////////////////////////////
// // /
// int readC02()
// {
//  // start I2C
// // © June 26, 2018 CO2Meter, Inc. All Rights Reserved    5 
//  Wire.beginTransmission(ADDR_6713);
//  Wire.write(0x04); Wire.write(0x13); Wire.write(0x8B);
// Wire.write(0x00); Wire.write(0x01);
//  // end transmission
//  Wire.endTransmission();
//  // read report of current gas measurement in ppm
//  delay(2000);
//  Wire.requestFrom(ADDR_6713, 4); // request 4 bytes from slave device
//  data[0] = Wire.read();
//  data[1] = Wire.read();
//  data[2] = Wire.read();
//  data[3] = Wire.read();
//  Serial.print("Func code: "); Serial.print(data[0],HEX);
//  Serial.print(" byte count: "); Serial.println(data[1],HEX);
//  Serial.print("MSB: 0x"); Serial.print(data[2],HEX);
// Serial.print(" ");
//  Serial.print("LSB: 0x"); Serial.print(data[3],HEX);
// Serial.print(" ");
//  CO2ppmValue = ((data[2] * 0xFF ) + data[3]);
// }
// void loop() {
//  int co2Value =readC02();
//  {
//  Serial.print("CO2 Value: ");
//  Serial.println(CO2ppmValue);
//  }

//  delay(2000);
// } 



#include <Wire.h>

const int TC713_ADDR = 0x15; // I2C address of TC713
const int CO2_REG = 0x03;   // Register to read CO2 concentration

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  // Request CO2 data from TC713
  Wire.beginTransmission(TC713_ADDR);
  Wire.write(CO2_REG);
  Wire.endTransmission();

  // Read two bytes of CO2 data
  Wire.requestFrom(TC713_ADDR, 4);
  if (Wire.available() >= 4) {
    uint16_t co2Value = Wire.read() << 8 | Wire.read();
    Serial.print("CO2 Concentration: ");
    Serial.print(co2Value);
    Serial.println(" ppm");
  }

  delay(2000); // Delay before reading again
}
