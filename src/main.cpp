#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Midden van de schakelaar naar GND
// Zijkant van de schakelaar naar D pin. Verbind de kant waar de schakelaar in staat wanneer je de servo arm naar de servo toe wilt draaien.
// Werking tuimschakelaar https://th.indicatorlight.com/faq/how-to-wire-a-toggle-switch-with-4-prongs/

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
const int NumberOfServos = 13;
int Status[NumberOfServos];
int MinValue[NumberOfServos]={900,900,800,800,500,500,500,500,500,500,500,500,500};        //Naar zeide waar schuifbalk tegen behuizing komt
int MaxValue[NumberOfServos]={2100,2100,2000,1900,1900,1900,1900,1900,1900,1900,1900};     // Totale afstand = 2.370 stappen maar de start en einde zijn anders net hoe het tandwiel zit
int CurrentPosition[NumberOfServos];
int DelayTime[NumberOfServos]={30,30,30,30,30,30,30,30,30,30,30,30,30}; //maak groter dan 1 anders loop je tegen capaciteit problemen van de arduino
int StepSize[NumberOfServos]={30,30,30,30,30,30,30,30,30,30,30,30,30};
unsigned long previousMillis[NumberOfServos];


void moveServos(int inputPin, int ServoID){
    unsigned long currentMillis = millis();
    if((digitalRead(inputPin != Status[ServoID])) && (currentMillis-previousMillis[ServoID] >= DelayTime[ServoID])){
        previousMillis[ServoID] = currentMillis;
        
        if (digitalRead(inputPin) == 0){ //als tuimschakelaar weg van de kant van verbinding wijst die je niet verbonden hebt (verbinding wordt verbroken) dan servo draait de arm van zich af (afhankelijk van wissel hoe die komt te staan)
          if (CurrentPosition[ServoID] < MaxValue[ServoID]){
                CurrentPosition[ServoID] = CurrentPosition[ServoID] + StepSize[ServoID];
                pwm.writeMicroseconds(ServoID, CurrentPosition[ServoID]);
                // Serial.print("pin ");Serial.print(inputPin);Serial.print(" met servo");Serial.print(ServoID);Serial.print(" = "); Serial.println(CurrentPosition[ServoID]);

            }
          else{
 
                Status[ServoID]=1;
                
            }
        }
        else  //als tuimschakelaar naar kant van verbinding wijst 
        {
            if (CurrentPosition[ServoID] > MinValue[ServoID]){
                CurrentPosition[ServoID] = CurrentPosition[ServoID] - StepSize[ServoID];
                pwm.writeMicroseconds(ServoID, CurrentPosition[ServoID]);
                // Serial.print("pin ");Serial.print(inputPin);Serial.print(" met servo");Serial.print(ServoID);Serial.print(" = "); Serial.println(CurrentPosition[ServoID]);

            }
            else{
                Status[ServoID]=0;
                // Serial.println(Status[ServoID]);
                
            }
        }
    }
  
}

void setup(){
    pinMode(53, INPUT_PULLUP);
    pinMode(52,INPUT_PULLUP);
    pinMode(51,INPUT_PULLUP);
    pinMode(50,INPUT_PULLUP);
    pinMode(49, INPUT_PULLUP);
    pinMode(48,INPUT_PULLUP);
    pinMode(47,INPUT_PULLUP);
    pinMode(46,INPUT_PULLUP);
    pinMode(45,INPUT_PULLUP);
    pinMode(44, INPUT_PULLUP);
    pinMode(43,INPUT_PULLUP);
    pinMode(42,INPUT_PULLUP);
    pinMode(41,INPUT_PULLUP);
    Serial.begin(9600);
    // pinMode(6, INPUT_PULLUP);
    for(int i=0; i<NumberOfServos; i=i+1){
        CurrentPosition[i]=MinValue[i];
    }
    pwm.begin();
    pwm.setPWMFreq(50);
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
   
   moveServos(52,5);
   moveServos(48,6);
   moveServos(46,7);
   moveServos(45,8);
   moveServos(44,9);
   moveServos(43,10);
   moveServos(42,11);
   moveServos(41,12);
   
   
  //  moveServos()
}