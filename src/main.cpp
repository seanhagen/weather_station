// all the project setup stuff
#include "setup.h"

// everything else
#include "client.h"
#include "lightning.h"
#include "myBME280.h"
#include "rain.h"
#include "uv.h"
#include "wind_dir.h"
#include "wind_speed.h"

#define LED 2

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
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  delay(2000);
  digitalWrite(LED, HIGH); // Blink stat LED
  delay(2000);
  digitalWrite(LED, LOW);

  Serial.begin(115200);
  while (!Serial)
    ;
  randomSeed(micros());

  seconds = 0;
  lastSecond = millis();

  Wire.begin();
  // SPI.begin();

  setupWindDir();
  setupWindSpeed();
  setupRain();
  setupUV();
  setupLightning();
  // one sensor does three things! amazing!
  setupBME280();

  setupMqtt();

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

    Serial.println("----------------------");
#if defined(ENABLE_WIND_DIR)
    windDirInfo wd = windDirLoop();
    Serial.print("Wind direction: ");
    Serial.println(wd.name);
#endif
#if defined(ENABLE_WIND_SPEED)
    windSpeedInfo ws = windSpeedLoop(minutes_10m);
    Serial.print("Wind speed: ");
    Serial.println(ws.windSpeedMPH);
#endif
#if defined(ENABLE_RAIN)
    rainInfo rn = rainLoop();
    Serial.print("Rain this hour: ");
    Serial.println(rn.rainHour);
    Serial.print("Rain this day: ");
    Serial.print(rn.dailyRain);
#endif
#if defined(ENABLE_UV)
    uvInfo v = uvLoop();
    Serial.print("UVA: ");
    Serial.print(v.uva);
    Serial.print(", UVB: ");
    Serial.print(v.uvb);
    Serial.print(", UV Index: ");
    Serial.println(v.index);
#endif
#if defined(ENABLE_BME280)
    bme280Reading thp = loopBME280();
    Serial.print("Temperature: ");
    Serial.print(thp.tempC);
    Serial.print(" °C, humidity: ");
    Serial.print(thp.humidity);
    Serial.print(" %RH, air pressure: ");
    Serial.print(thp.pressure);
    Serial.println(" Pa");
#endif
#if defined(ENABLE_LIGHTNING)
    lightningInfo li = loopLightning();
    Serial.print("Lightning: ");
    if (!li.strike) {
      Serial.print(" no strike detected");
      if (!li.isNoise && !li.isDisturber)
        Serial.println("!");
      if (li.isNoise)
        Serial.println(" -- noise!");
      if (li.isDisturber)
        Serial.println(" -- disturber!");
    } else {
      Serial.print(" strike detected ");
      Serial.print(li.distance);
      Serial.println("km away!");
    }
#endif
    Serial.println("Info dump complete");
    mqttLoop();
  }
  // digitalWrite(LED, LOW);
  // delay(100);
  // digitalWrite(LED, HIGH); // Blink stat LED
}
