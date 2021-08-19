#include "client.h"

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

CCR(mqttServer, MQTT_SERVER)
CCP(networkName, WIFI_SSID)
CCP(networkPswd, WIFI_PASSWORD)

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Opening to MQTT with device name ");
  Serial.print(STR(DEVICE_NAME));
  while (!client.connect(STR(DEVICE_NAME))) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("\nCONNECTED!");
}

void setupMqtt() {
  Serial.print("Connecting to wifi network: ");
  Serial.println(networkName);

  WiFi.mode(WIFI_STA);
  WiFi.begin(networkName, networkPswd);

  IPAddress addr;
  addr.fromString(mqttServer);

  Serial.print("Connecting to MQTT server at ");
  Serial.print(mqttServer);
  Serial.print(":");
  Serial.println(MQTT_PORT);

  client.begin(addr, MQTT_PORT, net);

  connect();

  if (!client.publish("/weather/esp32", "{'wooo':true}")) {
    Serial.print("Unable to pubulish message:");
    lwmqtt_err_t err = client.lastError();
    Serial.println(err);
  } else {
    Serial.println("Message published!");
  }
}

void mqttLoop() {
  client.loop();
  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    client.publish("/weather/esp32", "{'ping':true}");
  }
}
