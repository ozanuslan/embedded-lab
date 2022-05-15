#include <Arduino.h>
#include <HX711.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LOADCELL_DOUT_PIN 4
#define LOADCELL_SCK_PIN 5

LiquidCrystal_I2C lcd(0x3f, 16, 2);

HX711 scale;
float calibration_factor = -21000;

void setup()
{
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.print("SETUP STARTED");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  scale.tare(); // Reset the scale to 0

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("KG:  ");
  lcd.setCursor(0, 1);
  lcd.print("LB:  ");
}

float cull(float x)
{
  if (x < 0)
  {
    return 0;
  }
  else
  {
    return x;
  }
}

void printLCD(float kg)
{
  if (kg < 40)
  {
    lcd.setCursor(5, 0);
    lcd.print("           ");
    lcd.setCursor(5, 0);
    lcd.print(kg, 1);
  }
  else
  {
    lcd.setCursor(5, 0);
    lcd.print("OVER 40 KG");
    lcd.setCursor(5, 1);
    lcd.print("           ");
  }
  if (kg < 40)
  {
    lcd.setCursor(5, 1);
    lcd.print("           ");
    lcd.setCursor(5, 1);
    lcd.print(kg * 2.2046, 1);
  }
}

void loop()
{
  Serial.print("Reading: ");
  float kg = scale.get_value() / calibration_factor;
  Serial.print(kg, 1);
  Serial.print(" ||| ");
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();

  printLCD(cull(kg));

  if (Serial.available())
  {
    char temp = Serial.read();
    if (temp == 's')
    {
      Serial.println("Resetting scale");
      scale.set_offset(scale.read_average(5));
      scale.tare();
    }
  }
  delay(300);
}