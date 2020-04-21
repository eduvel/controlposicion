#include <SPI.h>
#include "DACMCP4821.h"

#include <Servo.h>
#include "FloatCasero.h"
int contador=0;
unsigned char Buffer[4];
unsigned long lData;
//#include <avr/interrupt.h>
//#define ConAntiWindUp
int TicOk=0;
Servo myservo;  // create servo object to control a servo

int val;    // variable to read the value from the analog pin
const int Trigger = 4;   //Pin digital 2 para el Trigger del sensor
const int Echo = 5;   //Pin digital 3 para el Echo del sensor
float referencia=17;
float error;
float distancia;
float salida;
float u=0;
float u1=0;
float kp=-0.7875;//-1;//3;
float e=0;
float e1=0;
float e2=0;
float td=0;//.125;//.0125;//0.11;
float ti=1.25;//1.5;//83;//0.45;
float t=0.02;
float us = 0;
float es = 0;
float ud = 0;
float ui = 0;
float ui1 = 0;
float up = 0;

//
//     ******** MCP4821 Pinout ********
//     ********************************
//     SDI Pin to MOSI Pin 11 Arduino UNO
//     SCK Pin to SCK Pin 13 Arduino UNO

/* Configure CS,SHUTDOWN and LDAC Pins */

DACMCP4821 dac(10,8,9);
int outputAnalog;
byte ganancia;
void setup(){
// Inicializo el DAC
  dac.initDAC();
  Serial.begin(115200);//iniciailzamos la comunicación
  pinMode(Trigger, OUTPUT); //pin como salida
  pinMode(Echo, INPUT);  //pin como entrada
  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0
  myservo.attach(3);  
}
void loop(){
  if(Serial.available()>1){
    Serial.readBytes(Buffer,2);
    referencia=(Buffer[0]-48)*10+(Buffer[1]-48);
  }
    //referencia=Binary2Float(Buffer);
  //kp=-analogRead(A1)*5.0/1024;
  distancia= medirDistancia();
  error=(referencia-distancia)*100/20;
  e=error;
  up=kp*e;
#ifdef ConAntiWindUp
  if ((u != 100) && (u != 0))
    ui = ui1 + kp / ti*t*(e + e1) / 2;
  else
    ui = ui1;
#else
  ui = ui1 + kp / ti*t*(e + e1) / 2+es;
#endif //
  ud =  kp*td*(e - e1) / t;
  u = up+ ui + ud;
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
  Serial.println(kp);
  //EnviarFloat(&distancia);
  //salida=referencia; 
  val=map((int)salida, 0, 100, 5, 135);
  myservo.write(val);
  delay(19); 
  /* writeDAC returns the MSB byte for testing. You can choose in the 
     second parameter the Gain1x = 1 or Gain2x = 2 */
  outputAnalog=distancia*4095/30;
  byte msb = dac.writeDAC(outputAnalog,2);

}

float medirDistancia(){
  
  long t; //timepo que demora en llegar el eco
  float d,d1; //distancia en centimetros
 
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);
   t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
  d1 = (float)t/59;             //escalamos el tiempo a una distancia en cm
  if((d1<=100)&&(d1>1))
    d=d1;

  
  return d;
  }
