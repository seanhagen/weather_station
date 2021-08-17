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

/*
 * All pin references:
 * ESP32:
 *   https://learn.sparkfun.com/tutorials/micromod-esp32-processor-board-hookup-guide/all
 * MicroMod Weather Carrier Board:
 *   https://learn.sparkfun.com/tutorials/micromod-weather-carrier-board-hookup-guide#hardware-overview
 */

/***** RAIN ******/
// D1 - GPIO pin 27, pad 18 on the MicroMod Weather Carrier Board
#define RAIN_PIN 27

// Uncomment to enable reporting rain info
//#define ENABLE_RAIN

/***** WIND DIRECTION ******/
// A1 - GPIO pin 35, pad 38 on the MicroMod Weather Carrier Board (Input Only!)
#define VANE_PIN 35

// Uncomment to enable wind direction
//#define ENABLE_WIND_DIR

/***** WIND SPEED ******/
// D0 - GPIO pin 14, pad 10 on the MicroMod Weather Carrier Board
#define WSPEED_PIN 14

// Uncomment to enable collecting wind speed
//#define ENABLE_WIND_SPEED

/***** BME280: TEMPERATURE, HUMIDITY, AIR PRESSURE ******/
// Uncomment to enable collecting temperature, humidity, and air pressure
#define ENABLE_BME280

/***** LIGHTNING ******/
#define ENABLE_LIGHTNING

// G3 - GPIO pin 17, pad 46 on the MicroMod Weather Carrier Board
#define LIGHTNING_INTERRUPT_PIN 17

// G1 - GPIO pin 25, pad 42 on the MicroMod Weather Carrier Board
#define LIGHTNING_CS_PIN 25

#endif
