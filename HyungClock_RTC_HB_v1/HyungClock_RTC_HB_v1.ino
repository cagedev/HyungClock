// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int motor1aPin = 3; // H-bridge motor1, leg a
int motor1bPin = 4; // H-bridge motor1, leg b
int speed1Pin = 9; // H-bridge motor1, speed pin
int ledPin = 13; //LED

int SWITCHTIME = 2000;
int MOTORSPEED = 255;

int currentMinute = 0;
int previousMinute = 0;

void setup() {
#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Serial.begin(9600);
  delay(3000); // wait for console opening
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  pinMode( motor1aPin, OUTPUT );
  pinMode( motor1bPin, OUTPUT );
  pinMode( speed1Pin, OUTPUT );
  pinMode( ledPin, OUTPUT );
}

void loop() {
  DateTime now = rtc.now();
//  serialTimePrint(now);

  currentMinute = now.minute();
  if (currentMinute != previousMinute) {
Serial.print(now.minute(), DEC);
Serial.println(" switching");
    previousMinute = currentMinute;
    digitalWrite(ledPin, HIGH);
    setMotor(MOTORSPEED);
    MOTORSPEED = - MOTORSPEED;
    delay(SWITCHTIME);
  } else {
    digitalWrite(ledPin, LOW);
  }

  /*
  digitalWrite(ledPin, HIGH);
  setMotor(255);
  delay(SWITCHTIME);

  digitalWrite(ledPin, LOW);
  setMotor(-255);
  delay(SWITCHTIME);
  */

  delay(10);
}


void setMotor( int speed1) {
  if( speed1 < 0 ) {
    // drive backwards
    digitalWrite( motor1aPin, LOW );
    digitalWrite( motor1bPin, HIGH );
    analogWrite( speed1Pin, abs(speed1) );
  } else {
    // drive forward
    digitalWrite( motor1aPin, HIGH );
    digitalWrite( motor1bPin, LOW );
    analogWrite( speed1Pin, speed1 );
  }
}

void serialTimePrint(DateTime now) {
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
}

