#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <EEPROM.h>

// Midden van de schakelaar naar GND
// Zijkant van de schakelaar naar D pin. Verbind de kant waar de schakelaar in staat wanneer je de servo arm naar de servo toe wilt draaien.
// Werking tuimschakelaar https://th.indicatorlight.com/faq/how-to-wire-a-toggle-switch-with-4-prongs/
// Arduino Mega ---> communicatie via I2C ---> PCA9685 ---> Servo
/*
  Schakelaars overzicht (bovenaanzicht bedieningspaneel)
   +---------------------------+
   |
   |     _________________________________
   |   ⟋                                 ⟍                           _____________________
   | ⟋       _______________________.______⟍.                      ⟋      _______________
   |     ⟋                     d45 s6⟍   d44 s5                 ⟋       ⟋
   |    ________________________       ⟍       ⟍             ⟋       ⟋
   |  ⟋                         ⟍        ⟍       ⟍        ⟋       ⟋
   |                               ⟍        \       ⟍___.⟋______.⟋_______________________
   |                                 \       |        d42s11  d41s7 ______________________
   |                                  |      |                ⟋   ⟋
   |                                  |      |        _____.⟋__⟋__
   |                                  |      |      ⟋d48s8-d52s12  ⟍.___________________
   |         ____________            /      /    .⟋________.____     ⟍__________________
   |                     ⟍         ⟋      / d53⟋s4     d46s10⟍  ⟍  d43s9
   |         _______.______⟍.____⟋       /  ⟋                  ⟍  ⟍_____________________
   |            d51s2 ⟍   d49s0         / ⟋                       ⟍______________________
   |         ___________⟍.____________.⟋d47s3
   |             d50s1
   |
   |
   */
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
const int NumberOfServos = 13;

enum ServoStatus
{
  MOVING,
  AT_MIN,
  AT_MAX
};

struct ServoConfig
{
  ServoStatus status;
  int minValue; // Naar zeide waar schuifbalk tegen behuizing komt
  int maxValue;
  int currentPosition;
  int delayTime; // maak groter dan 1 anders loop je tegen capaciteit problemen van de arduino
  int stepSize;
  unsigned long previousMillis;
  int vorigeSchakelaarStatus;
};

ServoConfig servos[NumberOfServos] = {
    {AT_MIN, 900, 2100, 0, 30, 30, 0, HIGH},
    {AT_MIN, 900, 2100, 0, 30, 30, 0, HIGH},
    {AT_MIN, 800, 2000, 0, 30, 30, 0, HIGH},
    {AT_MIN, 800, 2100, 0, 30, 30, 0, HIGH},
    {AT_MIN, 500, 2100, 0, 30, 30, 0, HIGH},
    {AT_MIN, 500, 2100, 0, 30, 30, 0, HIGH}, // 5
    {AT_MIN, 500, 1900, 0, 30, 30, 0, HIGH}, // 6
    {AT_MIN, 400, 1800, 0, 30, 30, 0, HIGH},
    {AT_MIN, 600, 2000, 0, 30, 30, 0, HIGH},
    {AT_MIN, 500, 2200, 0, 30, 30, 0, HIGH},
    {AT_MIN, 700, 1900, 0, 30, 30, 0, HIGH},//10
    {AT_MIN, 700, 2100, 0, 30, 30, 0, HIGH},
    {AT_MIN, 600, 2000, 0, 30, 30, 0, HIGH} // 12
};

void slaServoPositiesOp()
{
  for (int i = 0; i < NumberOfServos; i++)
  {
    EEPROM.put(i * sizeof(int), servos[i].currentPosition);
  }
}

void laadServoPosities()
{
  for (int i = 0; i < NumberOfServos; i++)
  {
    int positie;
    EEPROM.get(i * sizeof(int), positie);

    if (positie >= servos[i].minValue &&
        positie <= servos[i].maxValue)
    {
      servos[i].currentPosition = positie;
    }
    else
    {
      servos[i].currentPosition = servos[i].minValue;
    }

    pwm.writeMicroseconds(i, servos[i].currentPosition);
  }
}


void moveServos(int inputPin, int ServoID)
{
  if (ServoID < 0 || ServoID >= NumberOfServos)
    return;

  int huidigeSchakelaarStatus = digitalRead(inputPin);

  servos[ServoID].vorigeSchakelaarStatus = huidigeSchakelaarStatus;

  int doelpositie = (huidigeSchakelaarStatus == LOW)
                    ? servos[ServoID].maxValue
                    : servos[ServoID].minValue;

  unsigned long currentMillis = millis();

  if (currentMillis - servos[ServoID].previousMillis < servos[ServoID].delayTime)
    return;

  if (servos[ServoID].currentPosition == doelpositie)
    return;

  servos[ServoID].previousMillis = currentMillis;

  if (servos[ServoID].currentPosition < doelpositie)
  {
    servos[ServoID].currentPosition = min(
      servos[ServoID].currentPosition + servos[ServoID].stepSize,
      doelpositie
    );
  }
  else
  {
    servos[ServoID].currentPosition = max(
      servos[ServoID].currentPosition - servos[ServoID].stepSize,
      doelpositie
    );
  }

  pwm.writeMicroseconds(ServoID, servos[ServoID].currentPosition);

  if (servos[ServoID].currentPosition == doelpositie)
  {
    slaServoPositiesOp();
  }
}

const int treindetectiepins[] = {40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25};
  const int buzzerPin = A0;

  // Vorige status van iedere ingang
  int vorigeStatus[16];

  // Instellingen buzzer
  int buzzerFrequentie = 2000;  // Toonhoogte in Hz
  int buzzerDuur = 120;          // Pieplengte in milliseconden



  void setup()
{
  pinMode(53, INPUT_PULLUP);
  pinMode(52, INPUT_PULLUP);
  pinMode(51, INPUT_PULLUP);
  pinMode(50, INPUT_PULLUP);
  pinMode(49, INPUT_PULLUP);
  pinMode(48, INPUT_PULLUP);
  pinMode(47, INPUT_PULLUP);
  pinMode(46, INPUT_PULLUP);
  pinMode(45, INPUT_PULLUP);
  pinMode(44, INPUT_PULLUP);
  pinMode(43, INPUT_PULLUP);
  pinMode(42, INPUT_PULLUP);
  pinMode(41, INPUT_PULLUP);

  
  
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  // pinMode(6, INPUT_PULLUP);
  pwm.begin();
  pwm.setPWMFreq(50);

  // Servo-posities herstellen
  laadServoPosities();

  // Schakelaarstanden bij opstarten onthouden
  servos[0].vorigeSchakelaarStatus = digitalRead(49);
  servos[1].vorigeSchakelaarStatus = digitalRead(50);
  servos[2].vorigeSchakelaarStatus = digitalRead(51);
  servos[3].vorigeSchakelaarStatus = digitalRead(47);
  servos[4].vorigeSchakelaarStatus = digitalRead(53);
  servos[5].vorigeSchakelaarStatus = digitalRead(44);
  servos[6].vorigeSchakelaarStatus = digitalRead(45);
  servos[7].vorigeSchakelaarStatus = digitalRead(41);
  servos[8].vorigeSchakelaarStatus = digitalRead(48);
  servos[9].vorigeSchakelaarStatus = digitalRead(43);
  servos[10].vorigeSchakelaarStatus = digitalRead(46);
  servos[11].vorigeSchakelaarStatus = digitalRead(42);
  servos[12].vorigeSchakelaarStatus = digitalRead(52);
  
  // for (int i = 0; i < NumberOfServos; i = i + 1)
  // {
  //   servos[i].currentPosition = servos[i].minValue;
  //   pwm.writeMicroseconds(i, servos[i].currentPosition);
  // }
  for (int i = 0; i < 16; i++)
  {
    pinMode(treindetectiepins[i], INPUT_PULLUP);
    vorigeStatus[i] = digitalRead(treindetectiepins[i]);
  }
  Serial.print("Setup complete");
}





void TreinDetectie()
{
  for (int i = 0; i < 16; i++)
  {
    int huidigeStatus = digitalRead(treindetectiepins[i]);

    // Controleer of de ingang veranderd is
    if (huidigeStatus != vorigeStatus[i])
    {
      Serial.print("Pin ");
      Serial.print(treindetectiepins[i]);

      if (huidigeStatus == LOW)
      {
        Serial.println(" is aan GND!");
      }
      else
      {
        Serial.println(" is los!");
      }

      // Pieptoon bij iedere verandering
      tone(buzzerPin, buzzerFrequentie, buzzerDuur);

      // Vorige status bijwerken
      vorigeStatus[i] = huidigeStatus;
    }
  }

  // Uitgangen aansturen
  digitalWrite(7, digitalRead(39) == LOW ? LOW : HIGH);
  digitalWrite(6, digitalRead(34) == LOW ? LOW : HIGH);
  digitalWrite(13, digitalRead(38) == LOW ? LOW : HIGH);
}





void loop()
{
  TreinDetectie();
  Serial.println(digitalRead(53));
  //  moveServos(13,0); //input pin dus schakelaar en output pin van de servo op de PCA9685
  // 53-41
  moveServos(49, 0);
  moveServos(50, 1);
  moveServos(51, 2);
  moveServos(47, 3);
  moveServos(53, 4);
  // Doen het
  moveServos(45, 6);
  moveServos(44, 5);

  //    if((digitalRead(45)) == LOW){
  //     Serial.println("45");
  //    }
  // nog niet gemacht

  // emplacement
  moveServos(41, 7);
  moveServos(48, 8);
  moveServos(42, 11);
  moveServos(46, 10);
  moveServos(43, 9);
  moveServos(52, 12);

  //    moveServos(49,11);
  //    moveServos(45,12);
  //    moveServos(43,6);
  //    moveServos(42,9);

  //  moveServos()
}
