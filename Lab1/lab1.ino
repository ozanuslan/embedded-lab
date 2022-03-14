// Set pins 2-6 for the LEDs
// 2 is the least significant bit
// 6 is the most significant bit
const int pin0 = 2;
const int pin1 = 3;
const int pin2 = 4;
const int pin3 = 5;
const int pin4 = 6;

int mapPin(int pin)
{
  switch (pin)
  {
  case 0:
    return pin0;
  case 1:
    return pin1;
  case 2:
    return pin2;
  case 3:
    return pin3;
  case 4:
    return pin4;
  }

  return -1;
}

void turnOn(int pin)
{
  digitalWrite(mapPin(pin), HIGH);
}

void turnOff(int pin)
{
  digitalWrite(mapPin(pin), LOW);
}

void turnOnAll()
{
  turnOn(pin0);
  turnOn(pin1);
  turnOn(pin2);
  turnOn(pin3);
  turnOn(pin4);
}

void blinkPin(int pin, int d)
{
  turnOn(pin);
  delay(d);
  turnOff(pin);
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial)
  {
    ;
  }
  Serial.println("---SETUP START---");
  Serial.println("Serial has been set up");

  pinMode(pin0, OUTPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  Serial.println("Pin[0-4] initialized");

  Serial.println("---SETUP COMPLETE---\n");
}

int getCounterIncrement()
{
  int inc = 0;

  do
  {
    Serial.print("Please enter increment value [1-31]: ");
    while (!Serial.available())
    {
    }
    inc = Serial.parseInt();
    Serial.println();
    Serial.print("You've entered: ");
    Serial.println(inc);
    if (inc < 1 || inc > 31)
    {
      Serial.println("The value must be between 1 and 31");
    }
  } while (inc < 1 || inc > 31);

  return inc;
}

int getCounterDelay()
{
  int d = 0;

  do
  {
    Serial.print("Enter a delay(ms) for the counter [1-10000]: ");
    while (!Serial.available())
    {
    }
    d = Serial.parseInt();
    Serial.println();
    Serial.print("You've entered: ");
    Serial.println(d);
    if (d < 1 || d > 10000)
    {
      Serial.println("The delay must be between 1 and 10000");
    }
  } while (d < 1 || d > 10000);

  return d;
}

void setPin(int pin, int on)
{
  if (on)
  {
    turnOn(pin);
  }
  else
  {
    turnOff(pin);
  }
}

void displayIntOnPins(unsigned int num)
{
  int binaryDigits[5];
  for (int i = 0; i < 5; i++)
  {
    binaryDigits[i] = num % 2;
    num /= 2;
  }

  Serial.print("Binary: ");
  for (int i = 4; i >= 0; i--)
  {
    setPin(i, binaryDigits[i]);
    Serial.print(binaryDigits[i]);
  }
  Serial.println();
}

void count(unsigned int inc, int del)
{
  unsigned int count = 0;
  Serial.println("The counter is about to start, and count infinitely");
  Serial.println("If you would like to start a new counter please restart the Arduino");
  Serial.println("The counter starts in 3...");
  delay(1000);
  Serial.println("The counter starts in 2...");
  delay(1000);
  Serial.println("The counter starts in 1...");
  delay(1000);
  while (true)
  {
    Serial.print("Count:  ");
    Serial.println(count);
    displayIntOnPins(count);
    count += inc;
    count %= 32;
    delay(del);
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  Serial.println("Starting a fresh counting sequence");
  unsigned int inc = getCounterIncrement();
  unsigned int del = getCounterDelay();
  count(inc, del);
}
