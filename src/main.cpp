#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Midden van de schakelaar naar GND
// Zijkant van de schakelaar naar D pin. Verbind de kant waar de schakelaar in staat wanneer je de servo arm naar de servo toe wilt draaien.
// Werking tuimschakelaar https://th.indicatorlight.com/faq/how-to-wire-a-toggle-switch-with-4-prongs/

/*
  Schakelaars overzicht (bovenaanzicht bedieningspaneel)

   +---------------------------+
   |
   | __________________
   |                   ⟍
   | ______________.____.⟍
   |         d45 s6⟍  d44 s5
   | _________       ⟍       ⟍
   |          ⟍        ⟍       ⟍
   |            ⟍        \       ⟍___
   |              \       |       
   |               |      |
   */
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
const int NumberOfServos = 13;

enum ServoStatus {
  MOVING,
  AT_MIN,
  AT_MAX
};

struct ServoConfig {
  ServoStatus status;
  int minValue; //Naar zeide waar schuifbalk tegen behuizing komt
  int maxValue;
  int currentPosition;
  int delayTime; //maak groter dan 1 anders loop je tegen capaciteit problemen van de arduino
  int stepSize;
  unsigned long previousMillis;
};

ServoConfig servos[NumberOfServos] = {
  {AT_MIN, 900, 2100, 0, 30, 30, 0},
  {AT_MIN, 900, 2100, 0, 30, 30, 0},
  {AT_MIN, 800, 2000, 0, 30, 30, 0},
  {AT_MIN, 800, 1900, 0, 30, 30, 0},
  {AT_MIN, 500, 1900, 0, 30, 30, 0},
  {AT_MIN, 500, 1700, 0, 30, 30, 0}, //5 
  {AT_MIN, 700, 1900, 0, 30, 30, 0}, //6
  {AT_MIN, 500, 1900, 0, 30, 30, 0},
  {AT_MIN, 500, 1900, 0, 30, 30, 0},
  {AT_MIN, 500, 1900, 0, 30, 30, 0},
  {AT_MIN, 500, 1900, 0, 30, 30, 0},
  {AT_MIN, 500, 1900, 0, 30, 30, 0},
  {AT_MIN, 600, 2000, 0, 30, 30, 0}  //12
};


void moveServos(int inputPin, int ServoID) {
  if (ServoID < 0 || ServoID >= NumberOfServos) return;

  unsigned long currentMillis = millis();
  if (currentMillis - servos[ServoID].previousMillis < servos[ServoID].delayTime) {
    return;
  }
  servos[ServoID].previousMillis = currentMillis;

  bool switchToMax = (digitalRead(inputPin) == LOW); // richting

  if (switchToMax) {
    if (servos[ServoID].currentPosition < servos[ServoID].maxValue) {
      servos[ServoID].currentPosition += servos[ServoID].stepSize;
      servos[ServoID].status = MOVING;
    } else {
      servos[ServoID].currentPosition = servos[ServoID].maxValue;
      servos[ServoID].status = AT_MAX;
    }
  } 
  else {
    if (servos[ServoID].currentPosition > servos[ServoID].minValue) {
      servos[ServoID].currentPosition -= servos[ServoID].stepSize;
      servos[ServoID].status = MOVING;
    } else {
      servos[ServoID].currentPosition = servos[ServoID].minValue;
      servos[ServoID].status = AT_MIN;
    }
  }

  pwm.writeMicroseconds(ServoID, servos[ServoID].currentPosition);
//   if (servos[ServoID].status == MOVING) {
//   Serial.print("inputPin ");
//   Serial.print(inputPin);
//   Serial.print(" Servo ");
//   Serial.println(ServoID);
// }
}


void setup(){
    pinMode(53,INPUT_PULLUP);
    pinMode(52,INPUT_PULLUP);
    pinMode(51,INPUT_PULLUP);
    pinMode(50,INPUT_PULLUP);
    pinMode(49,INPUT_PULLUP);
    pinMode(48,INPUT_PULLUP);
    pinMode(47,INPUT_PULLUP);
    pinMode(46,INPUT_PULLUP);
    pinMode(45,INPUT_PULLUP);
    pinMode(44,INPUT_PULLUP);
    pinMode(43,INPUT_PULLUP);
    pinMode(42,INPUT_PULLUP);
    pinMode(41,INPUT_PULLUP);
    Serial.begin(9600);
    // pinMode(6, INPUT_PULLUP);
    pwm.begin();
    pwm.setPWMFreq(50);
    for(int i=0; i<NumberOfServos; i=i+1){
        servos[i].currentPosition=servos[i].minValue;
        pwm.writeMicroseconds(i, servos[i].currentPosition);
    }
    
}

void loop(){
  // Serial.println(digitalRead(53));
  //  moveServos(13,0); //input pin dus schakelaar en output pin van de servo op de PCA9685
  //53-41
   moveServos(49,0);
   moveServos(50,1);
   moveServos(51,2);
   moveServos(47,3);
   moveServos(53,4);
   //Doen het
   moveServos(45,6);
   moveServos(44,5);

//    if((digitalRead(45)) == LOW){
//     Serial.println("45");
//    }
//nog niet gemacht


   //emplacement
   moveServos(41,7); 
   moveServos(48,8);
   moveServos(42,11);
   moveServos(46,10);
   moveServos(43,9);
   moveServos(52,12);

   
//    moveServos(49,11);
//    moveServos(45,12);
//    moveServos(43,6);
//    moveServos(42,9);

   
   
  //  moveServos()
}