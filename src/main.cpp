#include <Arduino.h>
#include <station.h>

#define PERIOD 5000

Station *st;

long now = 0;
long lastSecond = 0;
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

  // setupMqtt();
}

void readMeasurements() {
  now = millis();
  if (now - lastSecond >= PERIOD) {
    digitalWrite(LED_BUILTIN, HIGH);

    if (second == 0) {
      Serial.println("Time  T      H      AP          W    UVA   UVB    UVI   "
                     "RAIN M  H  T  "
                     "WIND  N  MA  HA  LM LH LT");
    }

    second++;
    if (second > 59) {
      minute++;
      second = 0;
      if (minute > 59) {
        minute = 0;
      }
    }

    Serial.print(minute);
    Serial.print(":");
    Serial.print(second);
    Serial.print("   ");

    allMeasurements am;
    st->readAll(&am);

    Serial.print(am.immediate.temperature);
    Serial.print("  ");

    Serial.print(am.immediate.humidity);
    Serial.print("  ");

    Serial.print(am.immediate.pressure);
    Serial.print("  ");

    Serial.print(am.immediate.windDir);
    Serial.print("     ");

    Serial.print(am.immediate.uva);
    Serial.print("  ");

    Serial.print(am.immediate.uvb);
    Serial.print("  ");

    Serial.print(am.immediate.uvIndex);
    Serial.print("  ");

    Serial.print(am.counted.rainLastMinute);
    Serial.print("  ");

    Serial.print(am.counted.rainLastHour);
    Serial.print("  ");

    Serial.print(am.counted.rainToday);
    Serial.print("  ");

    Serial.print(am.counted.windSpeedNow);
    Serial.print("  ");

    Serial.print(am.counted.windSpeedMinuteAvg);
    Serial.print("  ");

    Serial.print(am.counted.windSpeedHourAvg);
    Serial.print("  ");

    Serial.print(am.counted.lightningLastMinute);
    Serial.print("  ");

    Serial.print(am.counted.lightningLastHour);
    Serial.print("  ");

    Serial.print(am.counted.lightningToday);

    Serial.println();

    lastSecond = now;
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void loop(void) {
  st->loop();
  readMeasurements();
}
