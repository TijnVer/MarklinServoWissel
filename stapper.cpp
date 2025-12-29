#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
const int NumberOfServos = 4;
int Status[NumberOfServos];
int MinValeu[NumberOfServos]={1000,1000,1000,0};
int MaxValeu[NumberOfServos]={1900,2000,2000,4096};
int CurrentPosition[NumberOfServos];
int DelayTime[NumberOfServos]={10,30,30,0};
int StepSize[NumberOfServos]={5,5,5,32};
unsigned long previousMillis[NumberOfServos];


void moveServos(int inputPin, int connection){
    unsigned long currentMillis = millis();
    if((digitalRead(inputPin != Status[connection]) && (currentMillis-previousMillis[connection] >= DelayTime[connection]))){
        previousMillis[connection] = currentMillis;
        if (digitalRead(inputPin) == 1){
            if (CurrentPosition[connection] < MaxValeu[connection]){
                CurrentPosition[connection] = CurrentPosition[connection] + StepSize[connection];
                pwm.writeMicroseconds(connection, CurrentPosition[connection]);
            }
            else{
                Status[connection]=1;
            }
        }
        else
        {
            if (CurrentPosition[connection] > MinValeu[connection]){
                CurrentPosition[connection] = CurrentPosition[connection] - StepSize[connection];
                pwm.writeMicroseconds(connection, CurrentPosition[connection]);
            }
            else{
                Status[connection]=0;
            }
        }
    }
}

void setup(){
    pinMode(5, INPUT_PULLUP);
    pinMode(6, INPUT_PULLUP);
    for(int i=0; i<NumberOfServos; i=i+1){
        CurrentPosition[i]=MinValeu[i];
    }
    pwm.begin();
    pwm.setPWMFreq(50);
}

void loop(){
   moveServos(5,0); //input pin dus schakelaar en output pin van de servo op de PCA9685

}