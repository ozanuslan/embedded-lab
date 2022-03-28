#include <Arduino.h>

const int buzzer_pin = 2;
const int sound_str_len = 201;
int sound_duration;
int silence_duration;
int finish_duration;
char sound_string[sound_str_len];

void setDefaultValues()
{
  sound_duration = 500;
  silence_duration = 50;
  finish_duration = 1000;
  strcpy(sound_string, "101011\0");
}

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("--SETUP BEGIN--");
  Serial.println("SERIAL INITIALIZED");
  pinMode(buzzer_pin, OUTPUT);
  Serial.println("PIN 1 SET TO OUTPUT");
  setDefaultValues();
  Serial.println("DEFAULT VALUES SET");
  Serial.println("--SETUP END--");
}

void waitSerialInput()
{
  while (!Serial.available())
    ;
}

void turnOn(int pin)
{
  digitalWrite(pin, HIGH);
}

void turnOff(int pin)
{
  digitalWrite(pin, LOW);
}

void printMenu()
{
  Serial.println("\n0 - PRINT THIS MENU");
  Serial.print("1 - SET SOUND   DURATION (MS): ");
  Serial.println(sound_duration);
  Serial.print("2 - SET SILENCE DURATION (MS): ");
  Serial.println(silence_duration);
  Serial.print("3 - SET FINISH  DURATION (MS): ");
  Serial.println(finish_duration);
  Serial.print("4 - SET SOUND   STRING       : ");
  Serial.println(sound_string);
  Serial.println("5 - PLAY SOUND");
  Serial.println();
  Serial.println("6 - RESET");
}

int getChoice(int min, int max, char *msg, char *err)
{
  int choice;

  do
  {
    Serial.print(msg);
    waitSerialInput();
    choice = Serial.parseInt();
    Serial.println(choice);
    if (choice < min || choice > max)
    {
      Serial.println(err);
    }
  } while (choice < min || choice > max);

  return choice;
}

void getSoundString(char *str)
{
  String buf = "";
  bool containsOtherThan0or1;
  bool isValidString = false;
  do
  {
    containsOtherThan0or1 = false;

    Serial.print("ENTER SOUND STRING (LENGTH [1-");
    Serial.print(sound_str_len - 1);
    Serial.print("]): ");
    waitSerialInput();
    buf = Serial.readString();
    Serial.println(buf);
    if (buf.length() > sound_str_len - 1 || buf.length() < 1)
    {
      Serial.print("\nINVALID TEXT LENGTH (");
      Serial.print(buf.length());
      Serial.println(")");
    }
    else
    {
      for (int i = 0; i < buf.length(); i++)
      {
        if (buf.charAt(i) != '0' && buf.charAt(i) != '1')
        {
          containsOtherThan0or1 = true;
        }
      }
      if (containsOtherThan0or1)
      {
        Serial.println("\nINVALID TEXT (CONTAINS CHARACTERS OTHER THAN 0 OR 1)");
      }
      else
      {
        isValidString = true;
      }
    }
  } while (!isValidString);
  strcpy(str, buf.c_str());
  str[buf.length()] = '\0';
}

int getMenuChoice()
{
  return getChoice(0, 6, "\nPLEASE ENTER MENU CHOICE [0-6]: ", "INVALID NUMBER, MUST BE BETWEEN 0 AND 6");
}

void displayCurrentOutput(int idx)
{
  const int len = strlen(sound_string);
  Serial.print("STRING: ");
  for (int i = 0; i < len; i++)
  {
    char c = sound_string[i];
    if (idx == i)
    {
      Serial.print(">");
      Serial.print(c);
      Serial.print("<");
    }
    else
    {
      Serial.print(c);
    }
  }
  Serial.println();
}

void playSound()
{
  const int str_len = strlen(sound_string);
  while (true)
  {
    for (int i = 0; i < str_len; i++)
    {
      displayCurrentOutput(i);
      if (sound_string[i] == '1')
      {
        turnOn(buzzer_pin);
        delay(sound_duration);
        turnOff(buzzer_pin);
        delay(silence_duration);
      }
      else
      {
        delay(sound_duration + silence_duration);
      }
    }
    Serial.println("FINISHED");
    delay(finish_duration);
  }
}

void menu()
{
  printMenu();
  int choice;
  do
  {
    choice = getMenuChoice();
    switch (choice)
    {
    case 0:
      printMenu();
      break;
    case 1:
      sound_duration = getChoice(10, 10000, "PLEASE ENTER SOUND DURATION (MS)[10-10000]: ", "INVALID NUMBER, MUST BE BETWEEN 0 AND 10000");
      break;
    case 2:
      silence_duration = getChoice(0, 10000, "PLEASE ENTER SILENCE DURATION (MS)[0-10000]: ", "INVALID NUMBER, MUST BE BETWEEN 0 AND 10000");
      break;
    case 3:
      finish_duration = getChoice(0, 10000, "PLEASE ENTER FINISH DURATION (MS)[0-10000]: ", "INVALID NUMBER, MUST BE BETWEEN 0 AND 10000");
      break;
    case 4:
      getSoundString(sound_string);
      break;
    case 5:
      Serial.println("STARTING SOUND IN 3..");
      delay(1000);
      Serial.println("STARTING SOUND IN 2..");
      delay(1000);
      Serial.println("STARTING SOUND IN 1..");
      delay(1000);
      playSound();
      break;
    case 6:
      setDefaultValues();
      break;
    }
  } while (true);
}

void loop()
{
  menu();
}