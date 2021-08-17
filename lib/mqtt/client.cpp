#include "client.h"

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

CCR(mqttServer, MQTT_SERVER)
CCP(networkName, WIFI_SSID)
CCP(networkPswd, WIFI_PASSWORD)

void connect(){
  Serial.print("Opening to MQTT with device name ");
  Serial.print(STR(DEVICE_NAME));
  while (!client.connect(STR(DEVICE_NAME))){
    Serial.print(".");
    delay(100);
  }
  Serial.println("\nCONNECTED!");
}

void setupMqtt() {
  Serial.println("Connecting to wifi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(networkName, networkPswd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  IPAddress addr;
  addr.fromString(mqttServer);
  Serial.print("Connecting to MQTT server at ");
  Serial.print(mqttServer);
  Serial.print(":");
  Serial.println(MQTT_PORT);
  
  client.begin(addr, MQTT_PORT, net);
  
  connect();
  
  if (!client.publish("/weather/esp32", "{'wooo':true}")){
    Serial.print("Unable to pubulish message:");
    lwmqtt_err_t err = client.lastError();
    Serial.println(err);
  } else {
    Serial.println("Message published!");
  }

}
