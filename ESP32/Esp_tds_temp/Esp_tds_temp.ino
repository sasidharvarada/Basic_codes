 #define SERIAL Serial
 #include "OneWire.h"
#include "DallasTemperature.h"
OneWire oneWire(27);
DallasTemperature tempSensor(&oneWire);
#define tdssensorPin 35
 
int tdssensorValue = 0;
float tdsValue = 0;
float Voltage = 0;
void setup() {
    
  Serial.begin(115200);
  tempSensor.begin();
}
void loop() {
    tdssensorValue = analogRead(tdssensorPin);
    Voltage = tdssensorValue*3.3/1024.0; //Convert analog reading to Voltage
    tdsValue=(133.42*Voltage*Voltage*Voltage - 255.86*Voltage*Voltage + 857.39*Voltage)*0.5; //Convert voltage value to TDS value
    Serial.print("TDS Value = "); 
    Serial.print(tdsValue);
    Serial.println(" ppm");
    tempSensor.requestTemperaturesByIndex(0);

    Serial.print("Temperature: ");
    Serial.print(tempSensor.getTempCByIndex(0));
    Serial.println(" C");
  
  delay(1000);
}