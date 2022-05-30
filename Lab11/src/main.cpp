#include <Arduino.h>
#include <L293.h>

#define A 4   // A segment on the 7-segment display
#define B 5   // B segment
#define C 6   // C segment
#define D 7   // D segment
#define E 8   // E segment
#define F 9   // F segment
#define G 10  // G segment
#define DP 11 // Dot on the corner of the LCD

#define LEFT_BUTTON A1  // Left button input on A1 pin
#define RIGHT_BUTTON A0 // Right button input on A0 pin

#define FWD_PIN 2
#define BCK_PIN 3
#define PWM_PIN 13

bool SEGMODE = 0;                    // 0 = Cathode, 1 = Anode
int SEGS[]{A, B, C, D, E, F, G, DP}; // segment pins
byte MAX_CHAR_VAL = 16;              // max value in the array "CHARS"
byte CHARS[16][9]{
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

void setSegmentState(bool mode) // sets the whole segment state to "mode"
{
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(SEGS[i], mode);
  }
}

void printSegment(int num) // print any number on the segment
{
  setSegmentState(SEGMODE); // turn off the segment
  if (num < 0)
  {
    digitalWrite(SEGS[7], HIGH); // turn on the dot
  }
  else
  {
    digitalWrite(SEGS[7], LOW); // turn off the dot
  }
  int n = num < 0 ? -num : num; // if num is negative, make it positive
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(SEGS[i], SEGMODE == 0 ? CHARS[n][i + 1] : !CHARS[n][i + 1]);
  }
}

bool LAST_RIGHT_READ = 1;
bool LAST_LEFT_READ = 1;
const int MAX_STEP = 4;
int STEP = 0;

// L293 motor(PWM_PIN, FWD_PIN, BCK_PIN);

void setup()
{
  Serial.begin(9600);

  for (int i = 0; i < 8; i++)
  {
    pinMode(SEGS[i], OUTPUT);
  }
  setSegmentState(SEGMODE); // turn off the segment

  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);

  pinMode(PWM_PIN, OUTPUT);
  pinMode(FWD_PIN, OUTPUT);
  pinMode(BCK_PIN, OUTPUT);

  digitalWrite(PWM_PIN, HIGH);
  // analogWrite(FWD_PIN, 800);
  digitalWrite(FWD_PIN, HIGH);
  digitalWrite(BCK_PIN, LOW);

  // analogWrite(BCK_PIN, 0);

  printSegment(STEP);
  // motor.forward(50);
}

void loop()
{
  //----- UPDATE BUTTON STATES -----//
  bool rightRead = digitalRead(RIGHT_BUTTON);
  bool leftRead = digitalRead(LEFT_BUTTON);
  bool isRightPressed = !LAST_RIGHT_READ && rightRead;
  bool isLeftPressed = !LAST_LEFT_READ && leftRead;
  LAST_RIGHT_READ = rightRead;
  LAST_LEFT_READ = leftRead;
  //-------------------------------//

  //----- UPDATE & PRINT STEP -----//
  if (!(isRightPressed && isLeftPressed)) // Both buttons pressed?
  {
    if (isRightPressed) // Right button to increase
    {
      STEP = STEP + 1 <= MAX_STEP ? STEP + 1 : STEP;
    }
    else if (isLeftPressed) // Left button to decrease
    {
      STEP = STEP - 1 >= -MAX_STEP ? STEP - 1 : STEP;
    }
    printSegment(STEP);
  }
  //-------------------------------//

  delay(50);
}