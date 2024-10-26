#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "myssid";         // Replace with your WiFi SSID
const char* password = "password";   // Replace with your WiFi password

#define channelID 2706752            // Replace with your ThingSpeak channel ID
const char mqttUserName[] = "GxgjOycwDyQRFxwkMh4eEQQ";  
const char clientID[] = "GxgjOycwDyQRFxwkMh4eEQQ"; 
const char mqttPass[] = "1BIyRjPZG1y8jAMIpGXFK6ig";  

const char* mqtt_server = "mqtt3.thingspeak.com";
const char* publishTopic = "channels/2706752/publish"; // Publishing to the channel

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastUploadedTime = 0;
const unsigned long postingInterval = 20L * 1000L; // 20 seconds interval

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
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000); // Retry every 5 seconds
    }
  }
}

void publishMessage(const char* topic, String payload, boolean retained) {
  if (client.publish(topic, payload.c_str()))
    Serial.println("Message published [" + String(topic) + "]: " + payload);
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
  
  if (millis() - lastUploadedTime > postingInterval) { 
    int sensorValue_1 = random(100); // Replace with actual sensor value
    int sensorValue_2 = random(100); // Replace with actual sensor value
    
    String dataText = String("field1=" + String(sensorValue_1) + "&field2=" + String(sensorValue_2) + "&status=MQTTPUBLISH");
    publishMessage(publishTopic, dataText, true);    
    
    lastUploadedTime = millis();
  }
}
