#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

#define SoundSensorPin 35  // GPIO pin for sound sensor
#define VREF  3.3          // Voltage reference for ESP32 (operating voltage)

// WiFi credentials
const char* ssid = "myssid"; // Replace with your WiFi SSID
const char* password = "password"; // Replace with your WiFi password

// MQTT credentials
#define channelID 2706752 // Replace with your ThingSpeak channel ID
const char mqttUserName[] = "NDwXDDU5KQQ9OzAjMSIxMQ4";  
const char clientID[] = "NDwXDDU5KQQ9OzAjMSIxMQ4"; 
const char mqttPass[] = "f46TMzy65Nd09RikOeVkjgtj";  

// MQTT broker settings and topics
const char* mqtt_server = "mqtt3.thingspeak.com";
const char* publishTopic = "channels/2706752/publish"; // Updated channel ID
const char* subscribeTopicFor_Command_1 = "channels/2706752/subscribe/fields/field1"; // Updated channel ID

const unsigned long postingInterval = 5L * 1000L; // Post data every 5 seconds.

// Variables
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastUploadedTime = 0;
float voltageValue, dbValue;

//==========================================
void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
}

//==========================================
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientID, mqttUserName, mqttPass)) {  
      Serial.println("connected");
      client.subscribe(subscribeTopicFor_Command_1); // Subscribe to topics here
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

//==========================================
void setup() {
  Serial.begin(9600);
  while (!Serial) delay(1);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  pinMode(SoundSensorPin, INPUT);  // Set sound sensor pin as input
  delay(3000);                      // Delay for system stability
}

//==========================================
void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  if (millis() - lastUploadedTime > postingInterval) {
    // Read sound levels
    voltageValue = analogRead(SoundSensorPin) / 4096.0 * VREF;
    dbValue = voltageValue * 50.0;
    
    Serial.println("Noise value: " + String(dbValue));
    
    // Prepare the data (sending only the sound value)
    String dataText = String("field1=" + String(dbValue) + "&status=MQTTPUBLISH");
    publishMessage(publishTopic, dataText, true);
    
    lastUploadedTime = millis();
  }
}

//==========================================
void callback(char* topic, byte* payload, unsigned int length) {
  String incomingMessage = "";
  for (int i = 0; i < length; i++) incomingMessage += (char)payload[i];
  
  Serial.println("Message arrived [" + String(topic) + "] " + incomingMessage);
  
  // Check the incoming message
  if (strcmp(topic, subscribeTopicFor_Command_1) == 0) {
    if (incomingMessage.equals("1")) Serial.println("subscribed"); // Handle received message
    else Serial.println("subscribed");
  }
}

//==========================================
void publishMessage(const char* topic, String payload, boolean retained) {
  if (client.publish(topic, payload.c_str()))
    Serial.println("Message published [" + String(topic) + "]: " + payload);
}
