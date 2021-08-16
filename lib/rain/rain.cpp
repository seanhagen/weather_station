#include "rain.h"

// 60 floating numbers to keep track of 60 minutes of rain
volatile float rainHour[60];
// [rain inches over the past hour)] -- the accumulated
// rainfall in the past 60 min
float rainin = 0;
// [rain inches so far today in local time]
volatile float dailyrainin = 0;

// volatiles are subject to modification by IRQs
volatile unsigned long raintime, rainlast, raininterval, rain;

byte rainMinutes;

void setupRain() {
  Serial.println("Setting up rain pins and interrupts");
  // input from wind meters rain gauge sensor
  pinMode(RAIN_PIN, INPUT_PULLUP);

  // attach external interrupt pins to IRQ functions
  attachInterrupt(RAIN_PIN, rainIRQ, FALLING);
}

// rainIRQ is an interrupt routine --these are called by the hardware
// interrupts, not by the main code
void rainIRQ() {
  Serial.println("Rain IRQ!");
  // Count rain gauge bucket tips as they occur
  // Activated by the magnet and reed switch in the rain gauge, attached to
  // input D2
  raintime = millis(); // grab current time
  raininterval =
      raintime - rainlast; // calculate interval between this and last event

  // ignore switch-bounce glitches less than 10mS after initial edge
  if (raininterval > 10) {
    dailyrainin += 0.011;           // Each dump is 0.011" of water
    rainHour[rainMinutes] += 0.011; // Increase this minute's amount of rain

    rainlast = raintime; // set up for next event
  }
}

void setRainMinute(byte m) { rainMinutes = m; }
void zeroRain(byte minutes) {
  rainHour[minutes] = 0; //
}

void zeroRainDay() { dailyrainin = 0; }

rainInfo rainLoop() {
  // Total rainfall for the day is calculated within the interrupt
  // Calculate amount of rainfall for the last 60 minutes
  rainin = 0;
  for (int i = 0; i < 60; i++)
    rainin += rainHour[i];

  return rainInfo{dailyrainin, rainin};
}
