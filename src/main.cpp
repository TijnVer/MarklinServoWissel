#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Servo instellingen
#define SERVOMIN  150 // minimale pulsbreedte
#define SERVOMAX  600 // maximale pulsbreedte

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  pwm.begin();
  pwm.setPWMFreq(50); // 50 Hz voor servo's
  delay(10);
}

void loop() {
  // Voorbeeld: beweeg 3 servo's naar verschillende posities
  pwm.setPWM(0, 0, SERVOMIN);   // Servo 1 naar minimum
  pwm.setPWM(1, 0, (SERVOMIN + SERVOMAX) / 2); // Servo 2 naar midden
  pwm.setPWM(2, 0, SERVOMAX);   // Servo 3 naar maximum
  delay(1000);

  // Optioneel: beweeg terug
  pwm.setPWM(0, 0, SERVOMAX);
  pwm.setPWM(1, 0, SERVOMIN);
  pwm.setPWM(2, 0, (SERVOMIN + SERVOMAX) / 2);
  delay(1000);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}