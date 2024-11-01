#include "arduino.h"
#include "Rele.h"

void seguro() {
  digitalWrite(VERDE, LOW);
  digitalWrite(AMARELO, HIGH);
  digitalWrite(VERMELHO, HIGH);
}

void cuidado() {
  digitalWrite(VERDE, HIGH);
  digitalWrite(AMARELO, LOW);
  digitalWrite(VERMELHO, HIGH);
}

void perigo() {
  digitalWrite(VERDE, HIGH);
  digitalWrite(AMARELO, HIGH);
  digitalWrite(VERMELHO, LOW);
}