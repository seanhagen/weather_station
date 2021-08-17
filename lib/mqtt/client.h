#ifndef CLIENT_H
#define CLIENT_H
#include "setup.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiClient.h>

#define MSG_BUFFER_SIZE (50)

void setupMqtt();

#endif
