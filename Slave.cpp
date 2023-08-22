#include <Arduino.h>

int mic = 6;
int count = 0; 

int pulseHigh; // Integer variable to capture High time of the incoming pulse
int pulseLow; // Integer variable to capture Low time of the incoming pulse
float pulseTotal; // Float variable to capture Total time of the incoming pulse
float frequency; // Calculated Frequency

int GasFW = A4;
int GasBW = A5; 
int Luce = A3; 
 
  

int LuceMain = 11;
int GasMain1 = 12; 
int SuonoMain = 6; 
int calibrazione = 7; 

int sensorMin_luci = 1023;        // minimum sensor value
int sensorMax_luci = 0;           // maximum sensor value

int sensorMin_gas1 = 1023;        // minimum sensor value
int sensorMax_gas1 = 0;           // maximum sensor value
int sensorMin_gas2 = 1023;        // minimum sensor value
int sensorMax_gas2 = 0;           // maximum sensor value



int valore_luci = 0;
int valore_gas1 = 0; 
int valore_gas2 = 0; 

void Luci(){
  valore_luci = analogRead(Luce);
  //Serial.println("Valore Luce:" + String(valore_luci));
}
void Gas(){
valore_gas1 = analogRead(GasFW);
valore_gas2 = analogRead(GasBW);
//Serial.println("Valore gas1:" + String(valore_gas1));
//Serial.println("Valore gas2:" + String(valore_gas2));
//Serial.println("_________________");
}
void calibrazione_luci(){ 
    // record the maximum sensor value
    if (valore_luci > sensorMax_luci) {
      sensorMax_luci = valore_luci;
    }
    // record the minimum sensor value
    if (valore_luci < sensorMin_luci) {
      sensorMin_luci = valore_luci;
    }
}
void calibrazione_gas1(){
  if (valore_gas1 > sensorMax_gas1) {
      sensorMax_gas1 = valore_gas1;
  }
    // record the minimum sensor value
    if (valore_gas1 < sensorMin_gas1) {
      sensorMin_gas1 = valore_gas1;
    }
}
void calibrazione_gas2(){
  if (valore_gas2 > sensorMax_gas2) {
      sensorMax_gas2 = valore_gas2;
    }
    // record the minimum sensor value
    if (valore_gas2 < sensorMin_gas2) {
      sensorMin_gas2 = valore_gas2;
    }
}
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 5000;  //the value is a number of milliseconds

void Suono(){
int i = 0; 

  // put your main code here, to run repeatedly:
    pulseHigh = pulseIn(mic,HIGH);
    pulseLow = pulseIn(mic,LOW);
    pulseTotal = pulseHigh + pulseLow; // Time period of the pulse in microseconds
    frequency=1000000/pulseTotal; // Frequency in Hertz (Hz)
    if(i<11){
   if(/*(frequenza > 1650 && frequenza < 4300) ||*/ (frequency > 3200 && frequency < 5000) /*|| (frequency2 > 3200 && frequency2 < 5000)*/){
      count ++; 
      if(count == 5){
      //Serial.println("TROVATO SUONO");
      digitalWrite(3, HIGH); 
      delay(1000);
      digitalWrite(3, LOW); 
     } 
     }else {
        count =0;
     
    }i++;
    }
}
short int flag=0; 
short int flag_gas = 0; 
short int flag_suono = 0; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  pinMode(mic, INPUT); 
  pinMode(3, OUTPUT); 
  
    pinMode(SuonoMain, OUTPUT); 
    digitalWrite(SuonoMain, LOW); 
    pinMode(LuceMain, OUTPUT);
    digitalWrite(LuceMain, LOW);

    pinMode(GasMain1, OUTPUT);
    digitalWrite(GasMain1, LOW); 
  
  
    pinMode(calibrazione, INPUT_PULLUP); 
  //digitalWrite(2, LOW); 
   

  while( millis() <3000){
    Luci();
      calibrazione_luci();
    Gas();
      calibrazione_gas1();
      calibrazione_gas2();
  }
  startMillis = millis();
}

void loop() {
    Luci();
  if(valore_luci > sensorMax_luci + 5 && flag==0){
      digitalWrite(LuceMain, HIGH);
    delay(2500); 
    flag = 1;
  }else if(valore_luci < sensorMax_luci +5){
    flag = 0; 
  }
  if(digitalRead(calibrazione) == HIGH){
    //Serial.println("GAS"); 
    Gas();
    if(currentMillis - startMillis >= period && flag_gas == 0 ){
        if((valore_gas1 > sensorMax_gas1  + 55 || valore_gas2 > sensorMax_gas2 +55 ) && flag_gas==0){
           digitalWrite(GasMain1, HIGH);
            //Serial.println("GAS TROVATO");
            flag_gas = 1;
            startMillis = millis();
        }
    }else if((valore_gas1 < sensorMax_gas1 ) || (currentMillis - startMillis >= period && flag_gas == 1) ){
        flag_gas = 0;
    }
  }
  Suono();
    
  }
    
    
  
