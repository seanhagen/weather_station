#ifndef CLIENT_H
#define CLIENT_H
#include "setup.h"
#include <Arduino.h>
// #include <MQTT.h>
#include <PubSubClient.h>
#include <WiFi.h>

#define MSG_BUFFER_SIZE (50)

#define BUFFER_SIZE 500

void mqttLoop();

class MqttPublisher {
public:
  void setup();
  void setStateTopic(String topic);
  void setAvailabilityTopic(String topic);
  void loop();

  bool publish(const char msg[], size_t length);
  bool publish(const char topic[], const char msg[], size_t length);

  bool publishValue(const char topic[], int val);
  bool publishValue(const char topic[], float val);

private:
  bool _haveStateTopic = false;
  String _stateTopic;

  bool _haveAvailTopic = false;
  String _availableTopic;

  void connect();
};

#endif
