#include <Arduino.h>

#define LED_PIN 12
#define SENSOR_PIN 11
#define A 3                     // A segment on the 7-segment display
#define B 4                     // B segment
#define C 5                     // C segment
#define D 6                     // D segment
#define E 7                     // E segment
#define F 8                     // F segment
#define G 9                     // G segment
#define DP 10                   // Dot on the corner of the LCD
#define DELAY_SECONDS 5         // Delay to be used in between motion detection
#define CIRCLE_DELAY_MILLIS 100 // Delay to be used in between each circle print cycle
#define SECOND_MILLIS 1000      // Milliseconds in a second

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
byte circleIdx = 0;  // Index of the current circle drawing cycle
bool last_state = 0; // Last state of the motion sensor

void setState(bool mode) // sets the whole segment state to "mode"
{
    for (int i = 0; i <= 6; i++)
    {
        digitalWrite(seg[i], mode);
    }
}

void printCircle(byte idx) // prints the circle at the index "idx"
{
    digitalWrite(seg[idx], !segMode);
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
    // pinMode(seg[7], OUTPUT); // uncomment if you want to use the DP (dot on the corner of the lcd)
    pinMode(SENSOR_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
}

void loop()
{
    bool state = digitalRead(SENSOR_PIN);
    bool isRisingEdge = state && !last_state;
    Serial.println(isRisingEdge ? "Motion Detected" : "No Motion");
    if (isRisingEdge)
    {
        int circleMillis;
        digitalWrite(LED_PIN, HIGH);
        for (int i = DELAY_SECONDS; i >= 0; i--) // print
        {
            Serial.println("Seconds left: " + String(i));
            if (i >= chars)
            {
                circleMillis = 0;
                while (circleMillis < SECOND_MILLIS)
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