#ifndef SETUP_H
#define SETUP_H
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#define ST(A) #A
#define STR(A) ST(A)
#define DO_EXPAND(VAL) VAL##1
#define EXPAND(VAL) DO_EXPAND(VAL)

#define CCP2(A, B) const char *A = B;
#define CCP(NAME, X) CCP2(NAME, STR(X))

#define CCR2(A, B) const char A[] = B;
#define CCR(NAME, X) CCR2(NAME, STR(X))

#if !defined(DEVICE_NAME) || (EXPAND(DEVICE_NAME) == 1)
#error You must define DEVICE_NAME, have you followed the instructions in README.md?
#endif

#if !defined(WIFI_SSID) || (EXPAND(WIFI_SSID) == 1)
#error You must define WIFI_SSID, have you followed the instructions in README.md?
#endif

#if !defined(WIFI_PASSWORD)
#error You must define WIFI_PASSWORD, have you followed the instructions in README.md?
#endif

#if !defined(MQTT_SERVER)
#error You must define MQTT_SERVER, have you followed the instructions in README.md?
#endif

#if !defined(MQTT_PORT) || (EXPAND(MQTT_PORT) == 1)
#define MQTT_PORT 1883
#endif

#endif
