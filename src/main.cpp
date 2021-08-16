#include "SparkFunBME280.h"
#include "SparkFun_AS3935.h"
#include "SparkFun_VEML6075_Arduino_Library.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include "rain.h"
#include "wind_dir.h"
#include "wind_speed.h"

#define LED 2

BME280 tempSensor;
VEML6075 uv;
SparkFun_AS3935 lightning;

// Global Variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
long lastSecond;  // The millis counter to see when a second rolls by
byte seconds;     // When it hits 60, increase the current minute
byte seconds_2m;  // Keeps track of the "wind speed/dir avg" over last 2 minutes
                  // array of data
byte minutes;     // Keeps track of where we are in various arrays of data
byte minutes_10m; // Keeps track of where we are in wind gust/dir over last 10
                  // minutes array of data

byte hour;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    ;

  seconds = 0;
  lastSecond = millis();

  pinMode(LED, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Wire.begin();
  // SPI.begin();
  setupWindDir();
  setupWindSpeed();
  setupRain();

  interrupts();
}

void loop(void) {
  if (millis() - lastSecond >= 1000) {
    lastSecond += 1000;

    // Take a speed and direction reading every second for 2 minute average
    if (++seconds_2m > 119)
      seconds_2m = 0;

    if (++seconds > 59) {
      seconds = 0;

      if (++minutes > 59) {
        minutes = 0;
        if (++hour > 23) {
          hour = 0;
          zeroRainDay();
        }
      }
      if (++minutes_10m > 9)
        minutes_10m = 0;

      setRainMinute(minutes);
      zeroRain(minutes);
      zeroWindGust10m(minutes_10m);
    }

    windSpeedInfo ws = windSpeedLoop(minutes_10m);
    windDirInfo wd = windDirLoop();
    rainInfo rn = rainLoop();

    Serial.println("----------------------");
    Serial.print("Wind direction: ");
    Serial.println(wd.name);
    Serial.print("Wind speed: ");
    Serial.println(ws.windSpeedMPH);
    Serial.print("Rain this hour: ");
    Serial.println(rn.rainHour);
    Serial.print("Rain this day: ");
    Serial.println(rn.dailyRain);
  }
  digitalWrite(LED, LOW);
  delay(100);
  digitalWrite(LED, HIGH); // Blink stat LED
}
