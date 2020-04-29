#include <TimerOne.h>
#include "FloatCasero.h"
#define _PID
#define _MAX_PERIODO 200
const byte ledPin = 9;
const byte interruptPin = 2;
volatile byte state = LOW;
int porcentajeSalida=30;
int periodoSalida;
int iTemperatura=0;
float fTemperatura=40;
float referencia=40;
float error;
float salida;
int Filtro[500];
float aData;
volatile int TicOk=0;
unsigned char Buffer[4];
float e=0;
unsigned long lData;
int cTic=0;
long Contador=0;
float u=10;
float u1=0;
float kp=13.23;//3;
float e1=0;
float e2=0;
float es=0;
float us=0;
float td=29;//0.11;
float ti=116;//0.45;
float t=2;

float ud = 0;
float ui = 0;
float ui1 = 0;
float up = 0;


void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), interrupcionFlancoBajada, FALLING);
  Timer1.initialize(4000);//periodo de muestreo en microsegundos
  Timer1.attachInterrupt(Tic) ;
  
}

void loop() {
  if (TicOk!=0){
      TicOk=0;
      iTemperatura=analogRead(A0);
      cTic++;
      long iSuma=0;
      for(int k=498;k>=0;k--){
        Filtro[k+1]=Filtro[k];
      }
       Filtro[0]=iTemperatura;
      for(int k=0;k<=499;k++){
          iSuma=iSuma+Filtro[k];
      }
      fTemperatura=(float)iSuma/500;
      fTemperatura=fTemperatura/194*209;
  }
  if((Serial.available()>3) && (cTic>=500)){
       cTic=0;
  //Recibir datos desde simulink
      Serial.readBytes(Buffer,4);
      aData=Binary2Float(Buffer);
      referencia=aData;
  #ifdef _PID
      error=(referencia-fTemperatura)*100/265;
      e=error;
      up=kp*e;
      ui = ui1 + kp / ti*t*(e + e1) / 2+es;
      ud =  kp*td*(e - e1) / t;
      u = up+50;//+ ui + ud; // cuando es proporcional solo sumar el bias =50
      us=u;
      if (u>100)
          u=100;
      if (u<0)
          u=0; 
      es=u-us;  
      ui1 = ui;
      e2=e1;
      e1=e;
      u1=u;
      salida=u;
 #else
      salida=referencia;
 #endif
  

      porcentajeSalida=(int)salida;//map((int)salida, 0, 100, 0,_MAX_PERIODO);

       EnviarFloat(&fTemperatura);
    
}
  
  
  if(state){
      state=LOW;
      generarSalida(porcentajeSalida);
  }
  
}

void interrupcionFlancoBajada() {
  state=HIGH;
 }

void generarSalida(int valorSalida){
  if(valorSalida>=periodoSalida){
      digitalWrite(ledPin,HIGH);
      long tiempoInicial=millis();
      while(tiempoInicial>=(millis()-5));
      //delay(5);
      digitalWrite(ledPin,LOW);
  }
  periodoSalida++;
  if(periodoSalida>_MAX_PERIODO){
      periodoSalida=0;
    }
  }

  void Tic(){
    TicOk=1;
 
}
