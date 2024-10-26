//--------------------------------------------------------------------
// const char* ssid = "network"; // Replace with your WiFi SSID
// const char* password = "123654789"; // Replace with your WiFi password

const char* ssid = "myssid"; // Replace with your WiFi SSID
const char* password = "password"; // Replace with your WiFi password

// MQTT credentials
#define channelID 2706752 // Replace with your ThingSpeak channel ID
const char mqttUserName[] = "NDwXDDU5KQQ9OzAjMSIxMQ4";  
const char clientID[] = "NDwXDDU5KQQ9OzAjMSIxMQ4"; 
const char mqttPass[] = "f46TMzy65Nd09RikOeVkjgtj";  

// MQTT broker settings and topics
const char* mqtt_server = "mqtt3.thingspeak.com";

// Published settings
const char* publishTopic = "channels/2706752/publish"; // Updated channel ID

// Subscribed settings for Virtuino command 1
const char* subscribeTopicFor_Command_1 = "channels/2706752/subscribe/fields/field1"; // Updated channel ID

const unsigned long postingInterval = 5L * 1000L; // Post data every 20 seconds.

// Variables and includes
 #include <WiFi.h>  


#include <PubSubClient.h>
#include <WiFiClient.h>
WiFiClient espClient;
PubSubClient client(espClient);  // Download the library PubSubClient from the Arduino library manager

unsigned long lastUploadedTime = 0;

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
  randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
}

//=====================================
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
      Serial.println(" try again in 5 seconds"); // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//========================================= messageReceived
void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  
  // Check for Virtuino Command 1
  if (topic == subscribeTopicFor_Command_1) {
    Serial.println("Command 1 = " + payload);
    int v = payload.toInt();
  }
}

//========================================= setup
void setup() {
  Serial.begin(9600);
  while (!Serial) delay(1);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

//========================================= loop
void loop() {
  if (!client.connected()) reconnect();
  client.loop();
  
  if (millis() - lastUploadedTime > postingInterval) { // The uploading interval must be > 15 seconds 
    int sensorValue_1 = random(100); // Replace with your sensor value
    int sensorValue_2 = random(100); // Replace with your sensor value
    
    String dataText = String("field1=" + String(sensorValue_1) + "&field2=" + String(sensorValue_2) + "&status=MQTTPUBLISH");
    publishMessage(publishTopic, dataText, true);    
    lastUploadedTime = millis();
  }
}

//========================================= callback function
void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage += (char)payload[i];
  
  Serial.println("Message arrived [" + String(topic) + "] " + incommingMessage);
  
  // Check the incoming message
  if (strcmp(topic, subscribeTopicFor_Command_1) == 0) {
    if (incommingMessage.equals("1")) Serial.println("subscribed"); // Turn the LED on 
    else Serial.println("subscribed"); // Turn the LED off 
  }
}

//======================================= publishing as string
void publishMessage(const char* topic, String payload, boolean retained) {
  if (client.publish(topic, payload.c_str()))
    Serial.println("Message published [" + String(topic) + "]: " + payload);
}
