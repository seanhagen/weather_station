#include "mqttClient.h"

WiFiClient net;
// MQTTClient client;
PubSubClient client(net);

unsigned long lastMillis = 0;

CCR(mqttServer, MQTT_SERVER)
CCP(networkName, WIFI_SSID)
CCP(networkPswd, WIFI_PASSWORD)

void MqttPublisher::setup() {
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

  // client.begin(addr, MQTT_PORT, net);
  client.setServer(mqttServer, MQTT_PORT);

  connect();

  if (_haveAvailTopic) {
    if (!client.publish(_availableTopic.c_str(), "on")) {
      Serial.print("Unable to available message?");
      // lwmqtt_err_t err = client.lastError();
      // Serial.println(err);
    }
  }

  Serial.print("Buffer size for MQTT client: ");
  Serial.print(client.getBufferSize());
  Serial.println(" bytes");

  Serial.print("Changing buffer size to ");
  Serial.print(BUFFER_SIZE);
  Serial.print(" bytes, success: ");
  bool s = client.setBufferSize(BUFFER_SIZE);
  Serial.println(s);
}

void MqttPublisher::setStateTopic(String topic) {
  _stateTopic = topic;
  _haveStateTopic = true;
}

void MqttPublisher::setAvailabilityTopic(String topic) {
  _availableTopic = topic;
  _haveAvailTopic = true;
}

void MqttPublisher::loop() {
  if (!client.connected()) {
    connect();
  }
  client.loop();

  // // publish a message roughly every second.
  // if (millis() - lastMillis > 1000) {
  //   lastMillis = millis();
  //   if (!client.publish(_topic.c_str(), "{'ping':true}")) {
  //     Serial.print("Unable to publish message?");
  //     // lwmqtt_err_t err = client.lastError();
  //     // Serial.println(err);
  //   }
  // }
}

bool MqttPublisher::publish(const char msg[], size_t length) {
  if (_haveStateTopic) {
    return client.publish(_stateTopic.c_str(), msg, length);
  }
  return false;
}

bool MqttPublisher::publish(const char topic[], const char msg[], size_t length) { return client.publish(topic, msg, length); }

bool MqttPublisher::publishValue(const char topic[], int val) {
  char v[20];
  itoa(val, v, 10);
  size_t l = (size_t)strlen(v);
  return client.publish(topic, v, l);
}

bool MqttPublisher::publishValue(const char topic[], float val) {
  char v[20];
  sprintf(v, "%0.5f", val);
  size_t l = (size_t)strlen(v);
  return client.publish(topic, v, l);
}

void MqttPublisher::connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");

  Serial.print("WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Opening to MQTT with device name ");
  Serial.print(STR(DEVICE_NAME));
  while (!client.connect(STR(DEVICE_NAME))) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("\nCONNECTED!");
}
