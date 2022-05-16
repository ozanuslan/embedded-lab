#include <Arduino.h>

const int LED_PIN = 3;
const int LIGHT_SENSOR_PIN = A0;
const int LED_ACTIVATION_VALUE = 500;
const int SERIAL_OUTPUT_FREQUENCY = 500;
unsigned long prevMillis;
unsigned long currMillis;

void setup()
{
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  prevMillis = millis();
}

void loop()
{
  int lightReading = analogRead(LIGHT_SENSOR_PIN);
  currMillis = millis();
  bool ledOn = lightReading < LED_ACTIVATION_VALUE;
  digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
  if (currMillis - prevMillis >= SERIAL_OUTPUT_FREQUENCY)
  {
    Serial.print("LIGHT SENSOR: ");
    Serial.println(lightReading);
    Serial.print("LED   STATUS: ");
    Serial.println(ledOn ? "ON" : "OFF");
    Serial.println();
    prevMillis = currMillis;
  }
}
