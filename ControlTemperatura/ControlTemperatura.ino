#define _MAX_PERIODO 100
const byte ledPin = 9;
const byte interruptPin = 2;
volatile byte state = LOW;
int porcentajeSalida;
int periodoSalida;
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), interrupcionFlancoBajada, FALLING);
}

void loop() {
  porcentajeSalida=50;
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
      delay(5);
      digitalWrite(ledPin,LOW);
  }
  periodoSalida++;
  if(periodoSalida>_MAX_PERIODO){
      periodoSalida=0;
    }
  }
