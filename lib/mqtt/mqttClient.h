#ifndef CLIENT_H
#define CLIENT_H
#include "setup.h"
#include <Arduino.h>
// #include <MQTT.h>
#include <PubSubClient.h>
#include <WiFi.h>

#define MSG_BUFFER_SIZE (50)

void mqttLoop();

class MqttPublisher {
public:
  void setup();
  void setStateTopic(String topic);
  void setAvailabilityTopic(String topic);
  void loop();

  bool publish(const char msg[], size_t length);

private:
  bool _haveStateTopic = false;
  String _stateTopic;

  bool _haveAvailTopic = false;
  String _availableTopic;

  void connect();
};

#endif
