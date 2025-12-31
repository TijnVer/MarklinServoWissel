#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN 150
#define SERVOMAX 600

Adafruit_PWMServoDriver myServo = Adafruit_PWMServoDriver();

const uint8_t numberOfServos = 10;

const int ANGLE_0 = 0;
const int ANGLE_130 = 130;

uint16_t PULSE_0;
uint16_t PULSE_130;

struct ServoAnim {
  bool moving = false;
  uint16_t fromPulse = 0;
  uint16_t toPulse = 0;
  uint32_t startTime = 0;
  uint32_t duration = 1000;
  int steps = 25;
  int currentStep = 0;
  float stepSize = 0;         // nu float i.p.v. int16_t
  uint32_t lastStepTime = 0;
  bool state = false;         // 0 = 0 graden, 1 = 130 graden
};

ServoAnim servoAnims[numberOfServos];

void startServoMove(uint8_t servoNum) {
  ServoAnim &anim = servoAnims[servoNum];
  if (anim.moving) return;

  anim.fromPulse = anim.state ? PULSE_130 : PULSE_0;
  anim.toPulse   = anim.state ? PULSE_0 : PULSE_130;
  anim.startTime = millis();
  anim.duration  = 1000;
  anim.steps     = 25;
  anim.currentStep = 0;
  anim.stepSize  = (float)(anim.toPulse - anim.fromPulse) / anim.steps;
  anim.lastStepTime = millis();
  anim.moving = true;
  anim.state = !anim.state;  // toggelen voor de volgende keer
}

void startServoMoveTo(uint8_t servoNum, bool to130) {
  ServoAnim &anim = servoAnims[servoNum];
  if (anim.moving) return;

  anim.fromPulse = anim.state ? PULSE_130 : PULSE_0;
  anim.toPulse   = to130 ? PULSE_130 : PULSE_0;
  anim.startTime = millis();
  anim.duration  = 1000;
  anim.steps     = 25;
  anim.currentStep = 0;
  anim.stepSize  = (float)(anim.toPulse - anim.fromPulse) / anim.steps;
  anim.lastStepTime = millis();
  anim.moving = true;
  anim.state = to130; // eindpositie vastleggen
}

void updateServoMoves() {
  uint32_t now = millis();
  for (uint8_t i = 0; i < numberOfServos; i++) {
    ServoAnim &anim = servoAnims[i];
    if (anim.moving) {
      uint32_t stepDelay = anim.duration / anim.steps;
      if (now - anim.lastStepTime >= stepDelay) {
        uint16_t pulse = anim.fromPulse + (int16_t)(anim.stepSize * anim.currentStep);
        myServo.setPWM(i, 0, pulse);
        anim.currentStep++;
        anim.lastStepTime = now;

        if (anim.currentStep >= anim.steps) {
          myServo.setPWM(i, 0, anim.toPulse); // eindpositie
          anim.moving = false;

          Serial.print("Servo ");
          Serial.print(i);
          Serial.print(" naar ");
          Serial.print(anim.state ? ANGLE_130 : ANGLE_0);
          Serial.println(" graden.");
        }
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  // while (!Serial); // Laat uit op een Mega (kan blokkeren)

  Serial.println("PCA9685 Servo Parallel Toggle Test");
  Wire.begin();
  myServo.begin();
  myServo.setPWMFreq(60);
  delay(10);

  // Pulsewaarden berekenen
  PULSE_0   = map(ANGLE_0, 0, 180, SERVOMIN, SERVOMAX);
  PULSE_130 = map(ANGLE_130, 0, 180, SERVOMIN, SERVOMAX);

  // Init alle servo's op beginpositie
  for (uint8_t i = 0; i < numberOfServos; i++) {
    myServo.setPWM(i, 0, PULSE_0);
    servoAnims[i].state = false;
  }
}

void loop() {
  static String input = "";

  // Seriële invoer verwerken
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r' || c == ' ' || c == ',') {
      input.trim();
      if (input.length() > 0) {
        if (input == "f") {
          startServoMoveTo(8, true);
          startServoMoveTo(9, true);
          startServoMoveTo(7, true);
        } else if (input == "g") {
          startServoMoveTo(8, false);
          startServoMoveTo(9, false);
          startServoMoveTo(7, false);
        } else {
          int servoNum = input.toInt();
          if (servoNum >= 0 && servoNum < numberOfServos) {
            startServoMove(servoNum);
          } else {
            Serial.print("Fout: typ een servonummer 0-");
            Serial.println(numberOfServos - 1);
          }
        }
        input = "";
      }
    } else {
      input += c;
    }
  }

  updateServoMoves();
  delay(2); // kleine delay om CPU te sparen
}

/*
Verbind de volgende pinnen:

UNO    ->  PCA9685
---------------------
SDA (A4) -> SDA
SCL (A5) -> SCL
5V       -> VCC
GND      -> GND

Let op:
- Servo's hebben vaak een aparte 5V voeding nodig.
- Verbind de GND van de servo-voeding met de GND van Arduino en PCA9685.
- De V+ pinnen op de PCA9685 zijn voor de servo-voeding.
*/
