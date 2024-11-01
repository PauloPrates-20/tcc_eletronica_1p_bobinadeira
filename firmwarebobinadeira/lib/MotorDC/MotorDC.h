#ifndef MotorDC.h
#define MotorDC.h
// Pinos do motor DC
#define PIN_MOTOR_DC 6
#define PIN_CLK 18

#include <Telas.h>
#include "Arduino.h"

// RPM inicial
extern int rpm;

// Parâmetros do encoder
extern volatile int pulsosEncoder = 0;
extern volatile bool ultimoEstadoClk = false;
extern unsigned long ultimoDebounce = 0;
extern const unsigned long DEBOUNCE = 0;

// Funções
void lerEncoder();
void ligarMotorDc();
void desligarMotorDc();
void medirRPMDC();

#include "Arduino.h"
#endif