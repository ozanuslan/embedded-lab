#include <Arduino.h>

#include <Servo.h>

#define A 3   // A segment on the 7-segment display
#define B 4   // B segment
#define C 5   // C segment
#define D 6   // D segment
#define E 7   // E segment
#define F 8   // F segment
#define G 9   // G segment
#define DP 10 // Dot on the corner of the LCD

#define SERVO_PIN 11
#define LEFT_BUTTON 12
#define RIGHT_BUTTON 13

bool segMode = 0;                   // 0 = Cathode, 1 = Anode
int seg[]{A, B, C, D, E, F, G, DP}; // segment pins
byte chars = 16;                    // max value in the array "Chars"
byte Chars[16][9]{
    // Corresponding low/high output for each hex number on the 7-segment display
    {'0', 1, 1, 1, 1, 1, 1, 0, 0}, // 0
    {'1', 0, 1, 1, 0, 0, 0, 0, 0}, // 1
    {'2', 1, 1, 0, 1, 1, 0, 1, 0}, // 2
    {'3', 1, 1, 1, 1, 0, 0, 1, 0}, // 3
    {'4', 0, 1, 1, 0, 0, 1, 1, 0}, // 4
    {'5', 1, 0, 1, 1, 0, 1, 1, 0}, // 5
    {'6', 1, 0, 1, 1, 1, 1, 1, 0}, // 6
    {'7', 1, 1, 1, 0, 0, 0, 0, 0}, // 7
    {'8', 1, 1, 1, 1, 1, 1, 1, 0}, // 8
    {'9', 1, 1, 1, 1, 0, 1, 1, 0}, // 9
    {'a', 1, 1, 1, 0, 1, 1, 1, 0}, // A/10
    {'b', 0, 0, 1, 1, 1, 1, 1, 0}, // b/11
    {'c', 1, 0, 0, 1, 1, 1, 0, 0}, // C/12
    {'d', 0, 1, 1, 1, 1, 0, 1, 0}, // d/13
    {'e', 1, 0, 0, 1, 1, 1, 1, 0}, // E/14
    {'f', 1, 0, 0, 0, 1, 1, 1, 0}, // F/15
};

void setState(bool mode) // sets the whole segment state to "mode"
{
  for (int i = 0; i <= 6; i++)
  {
    digitalWrite(seg[i], mode);
  }
}

void printSegment(int num) // print any number on the segment
{
  setState(segMode); // turn off the segment
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(seg[i], segMode == 0 ? Chars[num][i + 1] : !Chars[num][i + 1]);
  }
}

Servo myservo; // create servo object to control a servo

int pos = 0; // variable to store the servo position

void setup()
{
  Serial.begin(9600);
  myservo.attach(SERVO_PIN);

  for (int i = 0; i < 8; i++)
  {
    pinMode(seg[i], OUTPUT);
  }
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(seg[i], segMode);
  }

  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);

  digitalWrite(DP, HIGH);
}

bool last_right_read = 1;
bool last_left_read = 1;
byte count = 0;

void loop()
{
  bool right_read = digitalRead(RIGHT_BUTTON);
  bool left_read = digitalRead(LEFT_BUTTON);

  if (right_read == 0 && last_right_read == 1)
  {
    count = (count + 1) % chars;
    printSegment(count);
  }
  last_right_read = right_read;

  delay(100);
}