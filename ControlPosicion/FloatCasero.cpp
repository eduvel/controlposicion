
#include "FloatCasero.h"
#include <Arduino.h>
float signo;
float exponente;
float mantisa;
unsigned char *puntero;

float Binary2Float(unsigned char Dato[]){
    unsigned long BinaryDat;
    float signo;
    float exponente;
    float mantisa;
    unsigned char *puntero;
    float ResultadoFloat;
    BinaryDat=(long)Dato[3]*(1<<8)*(1<<8)*(1<<8)+(long)Dato[2]*(1<<8)*(1<<8)+(long)Dato[1]*(1<<8)+(long)Dato[0];
    if(Dato[3]& 0x80)
      signo=-1;
     else
      signo=1;
    exponente=((BinaryDat & 0x7F800000)>>23);
    exponente=exponente-127;
    mantisa=(BinaryDat & 0x007FFFFF)+0x00800000;
    mantisa=mantisa/0x800000;
    ResultadoFloat=signo*(mantisa*pow(2,exponente));
    return ResultadoFloat;
    }

 void EnviarFloat(float *fData){
    puntero=(long)fData;
    Serial.write(*puntero);
    puntero++;
    Serial.write(*puntero);
    puntero++;
    Serial.write(*puntero);
    puntero++;
    Serial.write(*puntero);
    puntero++;
    
  }
