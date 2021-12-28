#include "setup.h"

#define PERIOD 5000
#define SECOND 1000

Station *st;

MqttPublisher mClient;

long now = 0;
long lastSecond = 0;
long lastSecond2 = 0;
byte second = 0;
byte minute = 0;

bool setupComplete = false;

enum sensorType { SEN_RAIN, SEN_WIND, SEN_LIGHTNING, SEN_UV, SEN_ATMOS };

enum sensor { AIR_TEMP, AIR_HUM, AIR_PRE, UVA, UVB, RAIN_LM, RAIN_LH, RAIN_LD, WIND_N, WIND_MIN, WIND_HR, WIND_D, LS_LM, LS_LH, LS_LD };

typedef struct {
  int id;
  const char *objId;
  const char *name;
  sensorType type;
  const char *topic;
  const char *unit;
  const char *devClass;
  const char *icon;
} topicConfig;

const topicConfig topicDict[]{
    {AIR_TEMP, "6ce44d47", "Air Temperature", SEN_ATMOS, "homeassistant/sensor/weather_1", "°C", "temperature", "mdi:thermometer"},
    {AIR_HUM, "68f38d69", "Air Humidity", SEN_ATMOS, "homeassistant/sensor/weather_2", "RH", "humidity", "mdi:water-percent"},
    {AIR_PRE, "5c34091f", "Air Pressure", SEN_ATMOS, "homeassistant/sensor/weather_3", "hPa", "pressure", "mdi:air-filter"},
    {UVA, "df88a84d", "UVA", SEN_UV, "homeassistant/sensor/weather_4", "", "", "mdi:sun-wireless"},
    {UVB, "f2dd53c1", "UVB", SEN_UV, "homeassistant/sensor/weather_5", "", "", "mdi:sun-wireless-outline"},
    {RAIN_LM, "2b7273fa", "Rain - Last Minute", SEN_RAIN, "homeassistant/sensor/weather_6", "mm", "", "mdi:weather-rainy"},
    {RAIN_LH, "e16fe29d", "Rain - Last Hour", SEN_RAIN, "homeassistant/sensor/weather_7", "mm", "", "mdi:weather-rainy"},
    {RAIN_LD, "37d86cf0", "Rain - Last Day", SEN_RAIN, "homeassistant/sensor/weather_8", "mm", "", "mdi:weather-rainy"},
    {WIND_N, "5a91c496", "Wind - Now", SEN_WIND, "homeassistant/sensor/weather_9", "kph", "", "mdi:weather-windy"},
    {WIND_MIN, "edbbb0c3", "Wind - 1 Minute Average", SEN_WIND, "homeassistant/sensor/weather_10", "kph", "", "mdi:weather-windy"},
    {WIND_HR, "7f2284b3", "Wind - 1 Hour Average", SEN_WIND, "homeassistant/sensor/weather_11", "kph", "", "mdi:weather-windy"},
    {WIND_D, "4b8f5cee", "Wind Direction", SEN_WIND, "homeassistant/sensor/weather_12", "", "", "mdi:weather-windy"},
    {LS_LM, "975462e6", "Lightning Strikes - Last Minute", SEN_LIGHTNING, "homeassistant/sensor/weather_13", "#", "", "mdi:flash"},
    {LS_LH, "59986b7c", "Lightning Strikes - Last  Hour", SEN_LIGHTNING, "homeassistant/sensor/weather_14", "#", "", "mdi:flash"},
    {LS_LD, "8c909095", "Lightning Strikes - Last Day", SEN_LIGHTNING, "homeassistant/sensor/weather_15", "#", "", "mdi:flash"},
};

void firstTimeSetup(void) {
  stationConfig cnf;
  cnf.enableRain = true;
  cnf.enableWind = true;
  cnf.enableLightning = true;
  cnf.enableUV = true;
  cnf.enableAtmosphere = true;

  st = new Station(cnf);
  st->begin();

  mClient = MqttPublisher();
  mClient.setup();

  for (int i = 0; i < sizeof(topicDict) / sizeof(topicConfig); i++) {
    topicConfig cnf = topicDict[i];

    StaticJsonDocument<400> doc;
    doc.clear();
    doc["unique_id"] = cnf.objId;
    doc["~"] = cnf.topic;
    doc["stat_t"] = "~/state"; // state_topic
    doc["ic"] = cnf.icon;
    doc["name"] = cnf.name;

    doc["name"] = cnf.name;
    if ((cnf.unit != NULL) && (cnf.unit[0] != '\0')) {
      doc["unit_of_meas"] = cnf.unit; // unit_of_measurement
    }
    if ((cnf.devClass != NULL) && (cnf.devClass[0] != '\0')) {
      doc["dev_cla"] = cnf.devClass; // device_class
    }

    JsonObject device = doc.createNestedObject("dev");
    device["manufacturer"] = "SparkFun";
    device["model"] = "MicroMod Weather Station Carrier Board";
    device["via_device"] = "Mosquitto broker";

    JsonArray cons = device.createNestedArray("cns");
    cons.add(cnf.objId);

    char tpc[100];
    strcpy(tpc, cnf.topic);
    strcat(tpc, "/config");

    char buffer[500];
    // size_t n = serializeJson(doc, buffer);
    size_t n = serializeJsonPretty(doc, buffer);

    Serial.print("Configuring weather sensor '");
    Serial.print(cnf.name);
    Serial.print("', by sending json object to '");
    Serial.print(tpc);
    Serial.print("', and sending json data: '");
    Serial.print(buffer);
    Serial.print("' -- json size: ");
    Serial.print(n);
    Serial.println("!");
    mClient.publish(tpc, buffer, n);
  }
}

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  Serial.println("Hagen Weather Station starting up...");
  if (!setupComplete) {
    Serial.println("Running first time setup...");
    firstTimeSetup();
    setupComplete = true;
  }

  Serial.println("Setup complete!");
  digitalWrite(LED_BUILTIN, LOW);
}

void loop(void) {
  st->loop();
  mClient.loop();

  now = millis();
  if (now - lastSecond >= PERIOD) {
    digitalWrite(LED_BUILTIN, HIGH);

    allMeasurements am;
    st->readAll(&am);

    char windTopic[150];
    char topic[150];

    for (int i = 0; i < sizeof(topicDict) / sizeof(topicConfig); i++) {
      topicConfig cnf = topicDict[i];

      strcpy(topic, cnf.topic);

      bool publish = true;
      bool isFloat = true;
      float fval = 0;
      int ival = 0;

      switch (cnf.id) {
      case AIR_TEMP:
        fval = am.immediate.temperature;
        break;

      case AIR_HUM:
        fval = am.immediate.humidity;
        break;

      case AIR_PRE:
        fval = am.immediate.pressure;
        break;

      case UVA:
        fval = am.immediate.uva;
        break;

      case UVB:
        fval = am.immediate.uvb;
        break;

      case RAIN_LM:
        fval = am.counted.rainLastMinute;
        break;

      case RAIN_LH:
        fval = am.counted.rainLastHour;
        break;

      case RAIN_LD:
        fval = am.counted.rainToday;
        break;

      case WIND_N:
        fval = am.counted.windSpeedNow;
        break;

      case WIND_MIN:
        fval = am.counted.windSpeedMinuteAvg;
        break;

      case WIND_HR:
        fval = am.counted.windSpeedHourAvg;
        break;

      case LS_LM:
        isFloat = false;
        ival = am.counted.lightningLastMinute;
        break;

      case LS_LH:
        isFloat = false;
        ival = am.counted.lightningLastHour;
        break;

      case LS_LD:
        isFloat = false;
        ival = am.counted.lightningToday;
        break;

      case WIND_D:
        strcpy(windTopic, cnf.topic);
        strcat(windTopic, "/state");
        char *windDir = st->currentWindDirStr();
        mClient.publish(windTopic, windDir, (size_t)strlen(windDir));
        free(windDir); // don't forget to free this pointer, otherwise we'll run out of memory!
        publish = false;
      }
      strcat(topic, "/state");

      if (publish) {
        if (isFloat) {
          mClient.publishValue(topic, fval);
        } else {
          mClient.publishValue(topic, ival);
        }
      }
    }

    lastSecond = now;
    digitalWrite(LED_BUILTIN, LOW);
  }
}
