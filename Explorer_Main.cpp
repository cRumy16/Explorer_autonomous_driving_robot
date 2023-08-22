#include <Arduino.h>
//MOTORE DESTRA
int DXenA = 13;     
#define DX1 12     
#define DX2 11
//MOTORE SINSITRA
int SXenB = 8;      
#define SX3 10
#define SX4 9
/* CENTRALE - CENTRO_DESTRA - CENTRO_SINISTRA - LATERALE_DESTRA - LATERALE_SINISTRA */
int Trig[5]  {40,46,44,42,48};
int Echo[5]  {41,47,45,43,49};
 

int bafDX = 14; 
int bafSX = 15; 
int bafCE = 50; 
int mic = 6;
int Luce = 16; 
int Gas = 17; 

int led_luci = 4; 
int led_gas = 3;
int led_suono = 5; 

void SET_VELOCITA(int speed1, int speed2){
  analogWrite(DXenA,speed1);
  analogWrite(SXenB,speed2);
}

void FERMO(){
  digitalWrite(DX1, LOW);         
  digitalWrite(DX2, LOW);
  digitalWrite(SX3, LOW);
  digitalWrite(SX4, LOW);
} 
void AVANTI(){
  digitalWrite(DX1, LOW);         
  digitalWrite(DX2, HIGH);
  digitalWrite(SX3, LOW);
  digitalWrite(SX4, HIGH);
}
void SINISTRA(){
  //SET_VELOCITA(150,255);
  digitalWrite(DX1, LOW);         
  digitalWrite(DX2, HIGH);
  digitalWrite(SX3, HIGH);
  digitalWrite(SX4, LOW);
}
void DESTRA(){
  //SET_VELOCITA(255,150);
  digitalWrite(DX1, HIGH);         
  digitalWrite(DX2, LOW);
  digitalWrite(SX3, LOW);
  digitalWrite(SX4, HIGH);
}
void INDIETRO(){
  digitalWrite(DX1, HIGH);         
  digitalWrite(DX2, LOW);
  digitalWrite(SX3, HIGH);
  digitalWrite(SX4, LOW);
}

int DISTANZA(int trig, int echo){
  int dist;
  int duration;
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo, HIGH);
  dist= duration*0.034/2;
  return dist; 
}
int DIST[5]; //CENTRALE - CENTRO_DESTRA - CENTRO_SINISTRA - LATERALE_DESTRA - LATERALE_SINISTRA
void LEGGI(){
  for(int i=0; i<5; i++){
    DIST[i] = DISTANZA(Trig[i], Echo[i]);
  }
}
void RAD(int dist1, int dist2){
  unsigned int dist = dist1 - dist2; 
  if(dist <= 2 && dist1 > 4 && dist1 < 7 ){ // <7
    AVANTI();
  }
  else if(dist > 2 && dist < 100 && dist1 > 4 && dist1 < 7 ){ // <7
    if(dist1 < dist2){
    SINISTRA(); 
    }
    if(dist2 < dist1){
    DESTRA();
    }
    //AVANTI();
  }
  else {
      if(dist1 <= 3 ){
        SINISTRA();
        delay(30);
        AVANTI();
      }
      if(dist1 >= 10 && dist2 >= 10 ){ 
        if(dist > 2){
            if(dist1 < dist2){
                SINISTRA(); 
                }
            if(dist2 < dist1){
                DESTRA();
                }
        AVANTI();
        }else{
          DESTRA();
          delay(500);
          LEGGI();
          while( DIST[0] > 6 ) {
            LEGGI();
            AVANTI();
          }
          SINISTRA();
          delay(600);
          LEGGI();
          RAD(DIST[1], DIST[3]);
          } 
       }
    }

} 



unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 5000;  //the value is a number of milliseconds

void setup() {
  Serial.begin(9600);
  pinMode(DXenA, OUTPUT);
  pinMode(DX1, OUTPUT);
  pinMode(DX2, OUTPUT);

  pinMode(SXenB, OUTPUT);
  pinMode(SX3, OUTPUT);
  pinMode(SX4, OUTPUT);

  for(int i=0;i<5;i++){
   pinMode(Trig[i], OUTPUT);
   pinMode(Echo[i], INPUT);
  }
  
  pinMode(mic, INPUT); 
  digitalWrite(mic, LOW);
  pinMode(Luce, INPUT);
  digitalWrite(Luce, LOW); 
  pinMode(Gas, INPUT); 
  digitalWrite(Gas, LOW); 

  SET_VELOCITA(255,255);
  for(int i=3; i<6; i++)
    pinMode(i,OUTPUT);      // Imposto i pin dei led in OUTPUT

  for(int i=3; i<6;i++)
    digitalWrite(i, LOW);     // Imposto i pin dei led spenti}

  
  // Serial.println(" CALIBRATO"); 
  delay(3000);
  
  AVANTI();
}
bool sinistra = false; 
unsigned int distaDX = DIST[1] - DIST[3];
unsigned int distaSX = DIST[2] - DIST[4];

short int flag=0; 
short int flag_gas = 0; 
short int flag_suono = 0; 

void loop() {
  // int valoreSW = digitalRead(SwitchMode);
  // Serial.println(valoreSW);
  // if(valoreSW == HIGH){
  //   Serial.println("Sto calibrando");
  //   FERMO();
  //   Luci();
  //  // calibrazione(); 
  // }

// for(int i=0;i<5;i++){
// Serial.println( "Distanza sensore "  + String(i) + String(": ") + String(DISTANZA(Trig[i],Echo[i])) ); 
// }
// Serial.println("______________");  

//Serial.println("Valore sensorMax: " + String(sensorMax));

if (digitalRead(mic) == HIGH && flag_suono==0){
      digitalWrite(led_suono, HIGH);
      FERMO();
      delay(2500);
      AVANTI(); 
      digitalWrite(led_suono, LOW); 
      flag_suono=1; 
  }else if(digitalRead(mic) == LOW){
      digitalWrite(led_suono, LOW); 
      flag_suono = 0; 
  }


  if(digitalRead(Luce) == HIGH && flag==0){
     digitalWrite(led_luci, HIGH);
      FERMO();
      delay(2500);
      AVANTI();
      digitalWrite(led_luci, LOW); 
 
      flag=1; 
  }else if(digitalRead(Luce) == LOW){
      digitalWrite(led_luci, LOW); 
      flag_suono = 0; 
  }
  
     if(digitalRead(Gas) == HIGH && flag_gas==0){
     digitalWrite(led_gas, HIGH);
      FERMO();
      delay(2500);
      AVANTI(); 
      digitalWrite(led_gas, LOW); 

      flag_gas=1; 
  }else if(digitalRead(Gas) == LOW){
      digitalWrite(led_gas, LOW); 
      flag_gas = 0; 
  }
  
    
      //SET_VELOCITA(255,255);
LEGGI();
if(digitalRead(bafDX) == HIGH){
  //Serial.println("BAFFO DESTRO ATTIVO");
  //INDIETRO();
  while(distaDX >= 2 ){
    SINISTRA();
    LEGGI();
  }
  AVANTI();

}
if(digitalRead(bafSX) == HIGH){
  //Serial.println("BAFFO SINISTRO ATTIVO");
  //INDIETRO();
  while(distaDX >= 2 ){
    SINISTRA();
    LEGGI();
  }
  AVANTI();
  // sinistra= false; 
  // LEGGI();
  // if(DIST[2] > 0 && DIST[2] < 6 && sinistra == false){
  // DESTRA();
  // delay(1200);
  // LEGGI();
  // RAD(DIST[1], DIST[3]);
  // sinistra = true; 
}
if(digitalRead(bafCE) == HIGH){
  //Serial.println("BAFFO DESTRO ATTIVO");
  while(distaDX >= 2 ){
    SINISTRA();
    LEGGI();
  }
  AVANTI();
}
LEGGI();
if(DIST[2] > 0 && DIST[2] < 6 && sinistra == false){
  DESTRA();
  delay(1200);
  LEGGI();
  RAD(DIST[1], DIST[3]);
  sinistra = true; 
}
  if(DIST[0] > 6 && DIST[0] < 200){
    sinistra =true; 
  RAD(DIST[1], DIST[3]);
    if((DIST[1] - DIST[3]) > 10) {
      AVANTI();
      delay(700);
      DESTRA();
      delay(600);
      AVANTI();
      delay(650);
    } 
    RAD(DIST[1], DIST[3]);
  }
  else if(DIST[0] <= 6 && DIST[0] >0 ){
    SINISTRA();
    delay(600);
    AVANTI();
    LEGGI(); 
    //delay(300); 
    RAD(DIST[1], DIST[3]);
  }
  // else if(DIST[0] <=3){
  //   INDIETRO();
  //   delay(100);
  //   RAD(DIST[1], DIST[3]);
  // }
  //Suono();
  }
