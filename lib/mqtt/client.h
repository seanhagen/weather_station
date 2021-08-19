#ifndef CLIENT_H
#define CLIENT_H
#include "setup.h"
#include <MQTT.h>
#include <WiFi.h>

#define MSG_BUFFER_SIZE (50)

void setupMqtt();
void mqttLoop();

#endif
