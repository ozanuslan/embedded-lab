#include <LiquidCrystal.h>
#include <dht11.h>

const int rs = 12, en = 11, d4 = 9, d5 = 8, d6 = 7, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String floatToString(float f)
{
    char buf[6];
    dtostrf(f, 5, 1, buf);
    return String(buf);
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
int MILLIS = 0;
int DELAY = 20;

void setup()
{
    // Serial
    Serial.begin(9600);
    // Button input
    pinMode(BTNPIN, INPUT_PULLUP);
    // Temp sensor pins
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
    pinMode(3, OUTPUT);
    digitalWrite(3, HIGH);
    pinMode(4, INPUT);
    // LCD Begin
    lcd.begin(16, 2);
    lcd.print("INITIALIZED");
    Serial.println("SETUP COMPLETE");
}

void loop()
{
    int btn_in = digitalRead(BTNPIN);
    if (LAST_READ == 1 && btn_in == 0)
    {
        LANG = LANG ? 0 : 1;
    }
    LAST_READ = btn_in;

    Serial.println(LANG);

    delay(DELAY);

    MILLIS += DELAY;

    int chk = DHT11.read(DHT11PIN);
    if (chk < -1)
    {
        return;
    } // Failed to read correctly
    float hum = (float)DHT11.humidity;
    float c = (float)DHT11.temperature;
    float f = DHT11.fahrenheit();

    if (MILLIS >= 50)
    {
        lcd.clear();
        if (LANG == 0)
        {
            turkish(c, hum);
        }
        else
        {
            english(f, hum);
        }
        MILLIS = 0;
    }
}
