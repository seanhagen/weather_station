#define ARDUINOJSON_USE_DOUBLE 0
#include "ArduinoJson.h"
#include "mqttClient.h"
#include <Arduino.h>
#include <station.h>

#define PERIOD 5000
#define SECOND 1000

Station *st;

MqttPublisher mClient;

long now = 0;
long lastSecond = 0;
long lastSecond2 = 0;
byte second = 0;
byte minute = 0;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println("Setting up weather station!");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);

  stationConfig cnf;
  cnf.enableRain = false;
  cnf.enableWind = false;
  cnf.enableLightning = true;
  cnf.enableUV = true;
  cnf.enableAtmosphere = true;

  st = new Station(cnf);
  st->begin();

  mClient = MqttPublisher();
  mClient.setAvailabilityTopic("/weather/esp32/available");
  mClient.setStateTopic("/weather/esp32/data");
  mClient.setup();
}

void loop(void) {
  st->loop();
  mClient.loop();

  now = millis();
  if (now - lastSecond >= PERIOD) {
    digitalWrite(LED_BUILTIN, HIGH);

    allMeasurements am;
    st->readAll(&am);

    StaticJsonDocument<450> doc;
    doc["temperature"] = am.immediate.temperature;
    doc["humidity"] = am.immediate.humidity;
    doc["pressure"] = am.immediate.pressure;
    doc["uva"] = am.immediate.uva;
    doc["uvb"] = am.immediate.uvb;

    JsonObject rain = doc.createNestedObject("rain");
    rain["minute"] = am.counted.rainLastMinute;
    rain["hour"] = am.counted.rainLastHour;
    rain["day"] = am.counted.rainToday;

    JsonObject wind = doc.createNestedObject("wind");
    JsonObject speed = wind.createNestedObject("speed");
    speed["now"] = am.counted.windSpeedNow;
    speed["minute_avg"] = am.counted.windSpeedMinuteAvg;
    speed["hour_avg"] = am.counted.windSpeedHourAvg;

    char *windDir = st->currentWindDirStr();
    Serial.print("Wind dir string: '");
    Serial.print(windDir);
    Serial.println("' ");
    wind["direction"] = windDir;

    JsonObject lightning = doc.createNestedObject("lightning");
    lightning["minute"] = am.counted.lightningLastMinute;
    lightning["hour"] = am.counted.lightningLastHour;
    lightning["day"] = am.counted.lightningToday;

    JsonObject battery = doc.createNestedObject("battery");
    battery["level"] = 99.1;

    Serial.print("Publishing message to mqtt...");
    char buffer[500];
    size_t n = serializeJson(doc, buffer);
    bool success = mClient.publish(buffer, n);

    // don't forget to free this pointer, otherwise we'll run out of memory!
    free(windDir);

    Serial.print(" done, success: ");
    if (success) {
      Serial.print("SENT -- message size: ");
      Serial.println(n);
    } else {
      Serial.print("FAIL -- message we tried to send: ");
      Serial.println(buffer);
    }

    lastSecond = now;
    digitalWrite(LED_BUILTIN, LOW);
  }
}
