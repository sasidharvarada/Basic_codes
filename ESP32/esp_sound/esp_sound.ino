
//esp32
// #define SoundSensorPin 13  //this pin read the analog voltage from the sound level meter
// #define VREF  5.0  //voltage on AREF pin,default:operating voltage

//esp8266
#define SoundSensorPin A0  // ESP8266 uses A0 for analog input
#define VREF  3.3  // Voltage reference for ESP8266 (operating voltage)

float voltageValue,dbValue;

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    voltageValue = analogRead(SoundSensorPin) / 1024.0 * VREF;
    dbValue = voltageValue * 50.0;  //convert voltage to decibel value
    Serial.print(dbValue,1);
    Serial.println(" dBA");
    delay(125);
}