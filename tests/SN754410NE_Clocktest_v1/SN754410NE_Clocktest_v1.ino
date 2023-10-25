 // First introduce some handy constants
int motor1aPin = 3; // H-bridge motor1, leg a
int motor1bPin = 4; // H-bridge motor1, leg b
int speed1Pin = 9; // H-bridge motor1, speed pin
int ledPin = 13; //LED 

void setup() {
  // set outputs
  pinMode( motor1aPin, OUTPUT );
  pinMode( motor1bPin, OUTPUT );
  pinMode( speed1Pin, OUTPUT );
  pinMode( ledPin, OUTPUT );
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(ledPin, HIGH);
  setMotor(255);
  delay(2000);

  digitalWrite(ledPin, LOW);
  setMotor(-255);
  delay(2000);
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
