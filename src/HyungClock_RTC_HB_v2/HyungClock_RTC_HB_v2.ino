// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

int motor1aPin = 10;  // H-bridge motor1, leg a - IN1
int motor1bPin = 11;  // H-bridge motor1, leg b - IN2
int speed1Pin = 12;   // H-bridge motor1, speed/enable pin - EN

int ledPin = 13;  // Diagnotsic LED

int SWITCHTIME = 300;  // 300 ms for a "medium" pulse

int currentMinute = 0;
int previousMinute = 0;

void setup() {


#ifndef ESP8266
  while (!Serial)
    ;  // for Leonardo/Micro/Zero
#endif

  Serial.begin(9600);
  delay(3000);  // wait for console opening
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  pinMode(motor1aPin, OUTPUT);
  pinMode(motor1bPin, OUTPUT);
  pinMode(speed1Pin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Get current time
  DateTime now = rtc.now();

  // Extract current minute
  // currentMinute = now.minute();

  // Use quick minutes (3 seconds) for debugging
  currentMinute = now.second() / 3;
  // Serial.println(currentMinute);

  // Produce pulse if minute has changed
  if (currentMinute != previousMinute) {
    Serial.print(now.minute(), DEC);
    Serial.println(" switching");
    previousMinute = currentMinute;

    // Do a full pulse (+ then -)
    digitalWrite(ledPin, HIGH);
    // + part
    digitalWrite(speed1Pin, HIGH);
    digitalWrite(motor1aPin, HIGH);
    digitalWrite(motor1bPin, LOW);
    delay(SWITCHTIME);
    // - part
    digitalWrite(speed1Pin, HIGH);
    digitalWrite(motor1aPin, LOW);
    digitalWrite(motor1bPin, HIGH);
    delay(SWITCHTIME);

  } else {
    // Do nothing
    digitalWrite(ledPin, LOW);
    digitalWrite(speed1Pin, LOW);
    digitalWrite(motor1aPin, LOW);
    digitalWrite(motor1bPin, LOW);
  }

  delay(10);
}
