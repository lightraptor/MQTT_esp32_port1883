#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>


const char *SSID = "D_light";
const char *PWD = "23102002";

int mssv = 20520467;
int temp = 23;
int hum = 10;
int pres = 2002;

long last_time = 0;
char data[100];

// MQTT client
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient); 

const char *mqttServer = "public.mqtthq.com";
int mqttPort = 1883;


/*void connectToWiFi() {
  Serial.print("Connection to ");
 
  WiFi.begin(SSID, PWD);
  Serial.println(SSID);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.print("Connected.");
  
} 
*/
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    
  }
    Serial.println();
}

void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  // set the callback function
  mqttClient.setCallback(callback);
}


void setup() {
  Serial.begin(9600);
  delay(1000);

    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PWD);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
  setupMQTT();
    
}

void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
      
      if (mqttClient.connect(clientId.c_str())) {
        Serial.println("Connected.");
        // subscribe to topic
        mqttClient.subscribe("mr.Bach");
      }
      
  }
}


void loop() {

  if (!mqttClient.connected())
    reconnect();

  mqttClient.loop();
  long now = millis();
  if (now - last_time > 10000) {
    // Send data
    // Publishing data through MQTT
    sprintf(data, "%d", mssv);
    Serial.println(data);
    mqttClient.publish("dlight/00", data);
    sprintf(data, "%d", temp);
    Serial.println(data);
    mqttClient.publish("dlight/01", data);
    sprintf(data, "%d", hum);
    Serial.println(hum);
    mqttClient.publish("dlight/02", data);
    sprintf(data, "%d", pres);
    Serial.println(pres);
    mqttClient.publish("dlight/03", data);
    temp++;
    hum++;
    pres++;
    last_time = now;
  }

}