#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "myssid";         // Replace with your WiFi SSID
const char* password = "password";   // Replace with your WiFi password

#define channelID 2706752            // Replace with your ThingSpeak channel ID
const char mqttUserName[] = "AS0wOS8zFS8jLAgfCxIkACM";  
const char clientID[] = "AS0wOS8zFS8jLAgfCxIkACM"; 
const char mqttPass[] = "TzYXwXkQM3kbX2fqMh1BpX7y";  

const char* mqtt_server = "mqtt3.thingspeak.com";
const char* subscribeTopicFor_Command_1 = "channels/2706752/subscribe/fields/field1"; // Subscribing to field1

WiFiClient espClient;
PubSubClient client(espClient);

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

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientID, mqttUserName, mqttPass)) {  
      Serial.println("connected");
      client.subscribe(subscribeTopicFor_Command_1); // Subscribe to field1
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000); // Retry every 5 seconds
    }
  }
}

void messageReceived(char* topic, byte* payload, unsigned int length) {
  String incomingMessage = "";
  for (int i = 0; i < length; i++) {
    incomingMessage += (char)payload[i];
  }
  Serial.println("Message arrived [" + String(topic) + "] " + incomingMessage);
  
  if (strcmp(topic, subscribeTopicFor_Command_1) == 0) {
    if (incomingMessage.equals("1")) Serial.println("Command: ON"); 
    else Serial.println("Command: OFF");
  }
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(messageReceived);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
}
