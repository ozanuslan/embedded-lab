#include <Arduino.h>
#include <LiquidCrystal.h>
#include <dht11.h>

const int rs = 12, en = 11, d4 = 9, d5 = 8, d6 = 7, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String floatToString(float f)
{
  char buf[6];
  dtostrf(f, 5, 1, buf);
  String s = String(buf);
  if (LANG == 0)
  {
    s.replace(".", ",");
  }
  return s;
}

void turkish(float celc, float hum)
{
  lcd.setCursor(0, 0);
  lcd.print("SIC(C): ");
  lcd.print(floatToString(celc));
  lcd.print(" TR");
  lcd.setCursor(0, 1);
  lcd.print("NEM(%): ");
  lcd.print(floatToString(hum));
}

void english(float fah, float hum)
{
  lcd.setCursor(0, 0);
  lcd.print("TEM(F): ");
  lcd.print(floatToString(fah));
  lcd.print(" EN");
  lcd.setCursor(0, 1);
  lcd.print("HUM(%): ");
  lcd.print(floatToString(hum));
}

#define DHT11PIN 4
#define BTNPIN 13

dht11 DHT11;

int LANG = 1;
int LAST_READ = 1;

void setup()
{
  // Serial init
  Serial.begin(9600);
  // Button input pin
  pinMode(BTNPIN, INPUT_PULLUP);
  // Temp sensor pins
  pinMode(2, OUTPUT);    // GND pin
  digitalWrite(2, LOW);  // Output LOW
  pinMode(3, OUTPUT);    // VCC pin
  digitalWrite(3, HIGH); // Output HIGH
  pinMode(4, INPUT);     // Sensor pin
  // LCD Begin
  lcd.begin(16, 2);
  lcd.print("INITIALIZED LCD");
  Serial.println("SETUP COMPLETE");
}

void loop()
{
  // Detect language change
  int btn_in = digitalRead(BTNPIN);
  if (LAST_READ == 1 && btn_in == 0)
  {
    LANG = LANG ? 0 : 1;
  }
  LAST_READ = btn_in;

  // Try to read sensor data
  int chk = DHT11.read(DHT11PIN);
  if (chk < -1)
  {
    return;
  } // Failed to read correctly

  // Get sensor readings
  float hum = (float)DHT11.humidity;
  float c = (float)DHT11.temperature;
  float f = DHT11.fahrenheit();

  // Print to LCD
  lcd.clear();
  if (LANG == 0)
  {
    turkish(c, hum);
  }
  else
  {
    english(f, hum);
  }
}
