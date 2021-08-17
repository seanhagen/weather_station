#include "SparkFunBME280.h"
#include <Arduino.h>
#include <Wire.h>
BME280 mySensor;

/*
 * The anemometer outputs a digital signal tied to D0 (MicroMod pad 10).
 * The wind vain outputs an analog signal relative to its position tied to
 * A1 (MicroMod pad 38). The rainfall detector outputs a digital signal to
 * D1 (MicroMod pad 18).
 */

void setup() {
  Serial.begin(115200);
  Serial.println("Reading basic values from BME280");

  Wire.begin();
  if (mySensor.beginI2C() == false) { // Begin communication over I2C
    Serial.println("The sensor did not respond. Please check wiring.");
    while (1)
      ; // Freeze
  }
  // mySensor.setReferencePressure(101930);
}

void loop() {
  Serial.print("Humidity: ");
  Serial.print(mySensor.readFloatHumidity(), 0);

  Serial.print(" Pressure: ");
  Serial.print(mySensor.readFloatPressure(), 0);
  // // need to set reference pressure, otherwise will report incorrect
  altitude
      // Serial.print(" Alt: ");
      // Serial.print(mySensor.readFloatAltitudeMeters(), 1);
      // Serial.print(mySensor.readFloatAltitudeFeet(), 1);
      Serial.print(" Temp: ");
  Serial.print(mySensor.readTempC(), 2);
  // Serial.print(mySensor.readTempF(), 2);
  Serial.println();
  /*
Humidity: 43 Pressure: 101893 Alt: -154.7 Temp: 82.67
Humidity: 43 Pressure: 101895 Alt: -155.2 Temp: 82.67
*/
  delay(50);
}

// WiFi network name and password:
const char *networkName = "HagenIoT";
const char *networkPswd = "7cc3M7CptX";

// Internet domain to request from:
const char *hostDomain = "example.com";
const int hostPort = 80;

const int BUTTON_PIN = 0;
const int LED_PIN = ledPin;

void connectToWiFi(const char *, const char *);
void requestURL(const char *, uint8_t);
void printLine();

void setup() {
  // Initilize hardware:
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  // Connect to the WiFi network (see function below loop)
  connectToWiFi(networkName, networkPswd);

  digitalWrite(LED_PIN, LOW); // LED off
  Serial.print("Press the Boot button to connect to ");
  Serial.println(hostDomain);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) { // Check if button has been pressed
    while (digitalRead(BUTTON_PIN) == LOW)
      ; // Wait for button to be released

    digitalWrite(LED_PIN, HIGH);      // Turn on LED
    requestURL(hostDomain, hostPort); // Connect to server
    digitalWrite(LED_PIN, LOW);       // Turn off LED
  }
}

void connectToWiFi(const char *ssid, const char *pwd) {
  int ledState = 0;

  printLine();
  Serial.println("Connecting to WiFi network: " + String(ssid));

  WiFi.begin(ssid, pwd);

  while (WiFi.status() != WL_CONNECTED) {
    // Blink LED while we're connecting:
    digitalWrite(LED_PIN, ledState);
    ledState = (ledState + 1) % 2; // Flip ledState
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void requestURL(const char *host, uint8_t port) {
  printLine();
  Serial.println("Connecting to domain: " + String(host));

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  Serial.println("Connected!");
  printLine();

  // This will send the request to the server
  client.print((String) "GET / HTTP/1.1\r\n" + "Host: " + String(host) +
               "\r\n" + "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
  client.stop();
}

void printLine() {
  Serial.println();
  for (int i = 0; i < 30; i++)
    Serial.print("-");
  Serial.println();
}
