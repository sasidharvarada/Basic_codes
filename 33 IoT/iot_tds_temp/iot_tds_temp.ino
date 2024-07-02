#include <OneWire.h>
#include <DallasTemperature.h>


#define ONE_WIRE_BUS 14 // temp pin 

#define tdssensorPin 15 // tds pin 



OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);

int tdssensorValue = 0;
float tdsValue = 0;
float Voltage = 0;
float Temp = 0;


void setup() {
  Serial.begin(115200);
  tempSensor.begin();
}

void loop() {
 
 
 //tds  calibiration 
  tdssensorValue = analogRead(tdssensorPin);
  Voltage = tdssensorValue * 3.3 / 1024.0; // Convert analog reading to Voltage
    Serial.print(Voltage);
  tdsValue = (133.42 * Voltage * Voltage * Voltage - 255.86 * Voltage * Voltage + 857.39 * Voltage) * 0.5; // Convert voltage value to TDS value

  Serial.print("TDS Value = ");
  Serial.print(tdsValue);
  Serial.println(" ppm");
  tempSensor.requestTemperatures(); 
  
  // temp calibiration 
  Temp = tempSensor.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.print(Temp);
  Serial.println(" C");

  delay(1000);
}
