#ifndef MotorDC_h
#define MotorDC_h
// Pinos do motor DC
#define PIN_MOTOR_DC 6
#define PIN_CLK 18

#include <Telas.h>
#include "Arduino.h"

// RPM inicial
extern int rpm;

// Parâmetros do encoder
extern volatile int pulsosEncoder;
extern volatile bool ultimoEstadoClk;
extern unsigned long ultimoDebounce;
const unsigned long DEBOUNCE = 0;

// Funções
void lerEncoder();
void ligarMotorDc();
void desligarMotorDc();
void medirRPMDC();

#endif