#include "wind_speed.h"

#if defined(ENABLE_WIND_SPEED)
long lastWindCheck = 0;
volatile long lastWindIRQ = 0;
volatile byte windClicks = 0;

// We need to keep track of the following variables:
// Wind speed each update (no storage)
// Wind speed, avg over 2 minutes (store 1 per second)
// Wind gust over last 10 minutes (store 1 per minute)

byte windspdavg[120]; // 120 bytes to keep track of 2 minute average

int winddiravg[WIND_DIR_AVG_SIZE]; // 120 ints to keep track of 2 minute average
float windgust_10m[10];            // 10 floats to keep track of 10 minute max

// These are all the weather values that wunderground expects:
// [0-360 instantaneous wind direction]
int winddir = 0;
// [mph instantaneous wind speed]
float windspeedmph = 0;
// [mph current wind gust, using software specific time period]
float windgustmph = 0;
// [mph 2 minute average wind speed mph]
float windspdmph_avg2m = 0;
// [0-360 2 minute average wind direction]
int winddir_avg2m = 0;
// [mph past 10 minutes wind gust mph ]
float windgustmph_10m = 0;

void setupWindSpeed() {
  Serial.println("Setting up wind speed pins and interrupts");
  pinMode(WSPEED_PIN, INPUT_PULLUP);
  attachInterrupt(WSPEED_PIN, wspeedIRQ, FALLING);
}

void wspeedIRQ() {
  // Activated by the magnet in the anemometer (2 ticks per rotation),
  // attached to input D3
  // Ignore switch-bounce glitches less than 10ms (142MPH max reading)
  // after the reed switch closes
  if (millis() - lastWindIRQ > 10) {
    lastWindIRQ = millis(); // Grab the current time
    windClicks++;           // There is 1.492MPH for each click per second.
  }
}

windSpeedInfo windSpeedLoop(byte minutes_10m) {
  // Calc the wind speed and direction every second for 120 second to get 2
  // minute average
  float deltaTime = millis() - lastWindCheck; // 750ms

  deltaTime /= 1000.0; // Covert to seconds

  // Serial.println("-----------===========------------");
  // Serial.print("wind clicks: ");
  // Serial.print(windClicks);
  // Serial.print(", delta time: ");
  // Serial.print(deltaTime);
  // Serial.println();
  // Serial.println("-----------===========------------");

  float windSpeed = (float)windClicks / deltaTime; // 3 / 0.750s = 4

  windClicks = 0; // Reset and start watching for new wind
  lastWindCheck = millis();

  float currentSpeed = windSpeed * 1.492; // 4 * 1.492 = 5.968MPH

  /* Serial.println();
    Serial.print("Windspeed:");
    Serial.println(windSpeed);*/

  windspeedmph = currentSpeed; // update global variable for windspeed when
                               // using the printWeather() function

  bool isGust10min = false;
  // Check to see if this is a gust for the minute
  if (currentSpeed > windgust_10m[minutes_10m]) {
    windgust_10m[minutes_10m] = currentSpeed;
    isGust10min = true;
  }

  bool isGustDay = false;
  // Check to see if this is a gust for the day
  if (currentSpeed > windgustmph) {
    windgustmph = currentSpeed;
    isGustDay = true;
  }

  float temp = 0;
  for (int i = 0; i < 120; i++)
    temp += windspdavg[i];
  temp /= 120.0;
  windspdmph_avg2m = temp;

  return windSpeedInfo{windspeedmph, windgustmph, windspdmph_avg2m, isGust10min,
                       isGustDay};
}

void zeroWindGust10m(int i) {
  windgust_10m[i] = 0; // Zero out this minute's gust
}
#endif

#if !defined(ENABLE_WIND_SPEED)
void zeroWindGust10m(int i){}
void setupWindSpeed(){}
#endif
