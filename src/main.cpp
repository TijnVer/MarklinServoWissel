#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Midden van de schakelaar naar GND
// Zijkant van de schakelaar naar D pin

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
const int NumberOfServos = 4;
int Status[NumberOfServos];
int MinValeu[NumberOfServos]={610,710,710,710};        //Naar zeide waar schuifbalk tegen behuizing komt
int MaxValeu[NumberOfServos]={2980,3080,3080,3080};     // Totale afstand = 2.370 stappen maar de start en einde zijn anders net hoe het tandwiel zit
int CurrentPosition[NumberOfServos];
int DelayTime[NumberOfServos]={5,5,5,5}; //maak groter dan 1 anders loop je tegen capaciteit problemen van de arduino
int StepSize[NumberOfServos]={5,5,5,5};
unsigned long previousMillis[NumberOfServos];


void moveServos(int inputPin, int ServoID){
    unsigned long currentMillis = millis();
    if((digitalRead(inputPin != Status[ServoID]) && (currentMillis-previousMillis[ServoID] >= DelayTime[ServoID]))){
        previousMillis[ServoID] = currentMillis;
        
        if (digitalRead(inputPin) == 1){
          if (CurrentPosition[ServoID] < MaxValeu[ServoID]){
                CurrentPosition[ServoID] = CurrentPosition[ServoID] + StepSize[ServoID];
                pwm.writeMicroseconds(ServoID, CurrentPosition[ServoID]);
            }
          else{
                Status[ServoID]=1;
            }
        }
        else
        {
            if (CurrentPosition[ServoID] > MinValeu[ServoID]){
                CurrentPosition[ServoID] = CurrentPosition[ServoID] - StepSize[ServoID];
                pwm.writeMicroseconds(ServoID, CurrentPosition[ServoID]);
            }
            else{
                Status[ServoID]=0;
            }
        }
    }
  // Serial.print("Current position of servo ");Serial.print(ServoID);Serial.print(" = "); Serial.println(CurrentPosition[ServoID]);

}

void setup(){
    pinMode(53, INPUT_PULLUP);
    Serial.begin(9600);
    // pinMode(6, INPUT_PULLUP);
    for(int i=0; i<NumberOfServos; i=i+1){
        CurrentPosition[i]=MinValeu[i];
    }
    pwm.begin();
    pwm.setPWMFreq(50);
}

void loop(){
  Serial.println(digitalRead(53));
  //  moveServos(13,0); //input pin dus schakelaar en output pin van de servo op de PCA9685
   moveServos(53,0);
  //  moveServos(13,2);

}