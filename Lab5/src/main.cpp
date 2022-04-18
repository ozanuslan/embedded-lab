#include <Arduino.h>

#define LED_PIN 12
#define SENSOR_PIN 11

#define A 3
#define B 4
#define C 5
#define D 6
#define E 7
#define F 8
#define G 9
#define DP 10

bool segMode = 0;

#define DELAY_SECONDS 5
#define CIRCLE_DELAY_MILLIS 100
#define SECOND_MILLIS 1000

int seg[]{A, B, C, D, E, F, G, DP}; // segment pins
byte chars = 16;                    // max value in the array "Chars"
byte Chars[16][9]{
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
byte circleIdx = 0;
byte circle[8][8]{
    {1, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 1},
};
bool last_state = 0;

void setState(bool mode) // sets the hole segment state to "mode"
{
    for (int i = 0; i <= 6; i++)
    {
        digitalWrite(seg[i], mode);
    }
}

void printCircle(byte idx) // prints the circle at the index "idx"
{
    if (segMode == 0)
        digitalWrite(seg[idx], HIGH);
    else
        digitalWrite(seg[idx], LOW);
}

void print(int num) // print any number on the segment
{
    setState(segMode); // turn off the segment
    if (num >= chars)  // if the number is not declared
    {
        printCircle((circleIdx++) % 6); // print the circle
    }
    else // else if the number declared, print it
    {
        if (segMode == 0)
        { // for segment mode
            for (int i = 0; i < 8; i++)
            {
                digitalWrite(seg[i], Chars[num][i + 1]);
            }
        }
        else
        {
            for (int i = 0; i < 8; i++)
            {
                digitalWrite(seg[i], !Chars[num][i + 1]);
            }
        }
    }
}

void setup()
{
    Serial.begin(9600);
    pinMode(seg[0], OUTPUT);
    pinMode(seg[1], OUTPUT);
    pinMode(seg[2], OUTPUT);
    pinMode(seg[3], OUTPUT);
    pinMode(seg[4], OUTPUT);
    pinMode(seg[5], OUTPUT);
    pinMode(seg[6], OUTPUT);
    // pinMode(seg[7], OUTPUT);
    pinMode(SENSOR_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
}

void loop()
{
    bool state = digitalRead(SENSOR_PIN);
    Serial.println((last_state == 0 && state == 1) ? "Motion Detected" : "No Motion");
    if (last_state == 0 && state == 1)
    {
        int circleMillis;
        digitalWrite(LED_PIN, HIGH);
        for (int i = DELAY_SECONDS; i >= 0; i--) // print
        {
            Serial.println("Seconds left: " + String(i));
            if (i >= chars)
            {
                circleMillis = 0;
                while (circleMillis <= SECOND_MILLIS)
                {
                    print(i);
                    delay(CIRCLE_DELAY_MILLIS);
                    circleMillis += CIRCLE_DELAY_MILLIS;
                }
            }
            else
            {
                print(i);
                delay(SECOND_MILLIS);
            }
        }
        digitalWrite(LED_PIN, LOW);
    }
    setState(segMode);
    last_state = state;
}