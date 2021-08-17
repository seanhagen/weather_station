#ifndef SETUP_H
#define SETUP_H
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

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

#define ST(A) #A
#define STR(A) ST(A)

#ifdef DEVICE_NAME
#pragma message STR(DEVICE_NAME)
#endif

#ifdef WIFI_PASSWORD
#pragma message STR(WIFI_PASSWORD)
#endif

#ifdef WIFI_SSID
#pragma message STR(WIFI_SSID)
#endif

#ifdef MQTT_SERVER
#pragma message STR(MQTT_SERVER)
#endif

#ifdef MQTT_PORT
#pragma message STR(MQTT_PORT)
#endif

#endif
