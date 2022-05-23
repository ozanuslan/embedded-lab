#include <Arduino.h>
#include <HCSR04.h>
#include <LiquidCrystal_I2C.h>

const int LED_PIN = 2;
const int ECHO_PIN = 5;
const int TRIGGER_PIN = 6;

const int MAX_DIGITS = 6; // Max number of digits to display

LiquidCrystal_I2C lcd(0x3f, 16, 2);
UltraSonicDistanceSensor hc(TRIGGER_PIN, ECHO_PIN);

const int Buzzer_LED_Freq_Max = 1000; // ms
const int Buzzer_LED_Freq_Min = 10;   // ms
const int Update_Freq = 500;          // ms
const int Update_LCD = 200;           // ms
const int Dist_Max = 100;             // cm
const int Dist_Min = 10;              // cm
int Current_Dist = 0;                 // cm
int Current_Freq = 0;                 // ms

unsigned long Prev_Millis = 0; // ms timestamp of last loop iteration

int LCD_Timer = 0;     // time passed since last LCD update
int Freq_Timer = 0;    // time passed since last frequency update
int LED_Buz_Timer = 0; // time passed since last LED/Buzzer update

bool LED_Buz_On = false;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Distance :");
  lcd.setCursor(0, 1);
  lcd.print("Frequency:");
  Prev_Millis = millis();
}

float calcFreq(int cur_dist, int min_dist, int max_dist, int max_buz_freq, int min_buz_freq)
{
  return (((float)(cur_dist - min_dist) / (max_dist - min_dist)) * (max_buz_freq - min_buz_freq)) + min_buz_freq;
}

String floatToRightAlignedStr(float num)
{
  // first convert float to int
  // then convert int to string by using dtostrf
  int num_int = (int)num;
  char num_str[MAX_DIGITS];
  dtostrf(num_int, MAX_DIGITS, 0, num_str);
  return String(num_str);
}

void printLcd(float dist, float freq)
{
  lcd.setCursor(10, 0);
  lcd.print(floatToRightAlignedStr(dist));
  lcd.setCursor(10, 1);
  lcd.print(floatToRightAlignedStr(freq));
}

float decideRegulatedVal(float cur_val, float max_val, float min_val)
{
  if (cur_val > max_val)
  {
    return max_val;
  }
  else if (cur_val < min_val)
  {
    return min_val;
  }
  else
  {
    return cur_val;
  }
}

void loop()
{
  // Update current millis
  unsigned long curr_millis = millis();
  unsigned long time_passed = curr_millis - Prev_Millis;
  Prev_Millis = curr_millis;

  float raw_cur_dist = hc.measureDistanceCm();
  raw_cur_dist = raw_cur_dist < 0 ? Dist_Max : raw_cur_dist;
  float raw_cur_freq = calcFreq(raw_cur_dist, Dist_Min, Dist_Max, Buzzer_LED_Freq_Max, Buzzer_LED_Freq_Min);
  float cur_dist = decideRegulatedVal(raw_cur_dist, Dist_Max, Dist_Min);
  float cur_freq = decideRegulatedVal(raw_cur_freq, Buzzer_LED_Freq_Max, Buzzer_LED_Freq_Min);

  if (LCD_Timer >= Update_LCD) // update LCD
  {
    LCD_Timer %= Update_LCD;
    float display_dist = cur_dist;
    float display_freq = cur_freq;
    unsigned long before_lcd_print = millis();
    printLcd(display_dist, display_freq);
    curr_millis = millis();
    time_passed += millis() - before_lcd_print;
  }

  // advance timers
  LCD_Timer += time_passed;
  Freq_Timer += time_passed;
  LED_Buz_Timer += time_passed;

  if (Freq_Timer >= Update_Freq) // update distance and frequency values
  {
    Freq_Timer %= Update_Freq;
    Current_Dist = cur_dist;
    Current_Freq = cur_freq;
  }

  if (Current_Dist <= Dist_Min) // too close
  {
    digitalWrite(LED_PIN, HIGH);
  }
  else if (Current_Dist >= Dist_Max) // too far
  {
    digitalWrite(LED_PIN, LOW);
  }
  else if (LED_Buz_Timer >= Current_Freq) // if the timer has completed a cycle
  {
    LED_Buz_Timer %= Current_Freq;
    digitalWrite(LED_PIN, LED_Buz_On);
    LED_Buz_On = !LED_Buz_On;
  }
}