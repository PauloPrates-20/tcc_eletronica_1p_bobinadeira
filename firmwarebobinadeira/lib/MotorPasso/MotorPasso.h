#ifndef MotorPasso.h
#define MotorPasso.h
// Pinos do motor de passo
#define PWM_PASSO 5      // Pino PWM do motor de passo
#define DIRECAO_PASSO 4  // Pino de direção do motor de passo
#define ENABLE 2         // Pino enable do motor de passo
// Sentidos do motor de passo
#define FRENTE false
#define TRAS true
// Sensores
#define PORTA 12     // Pino do sensor de porta
#define INICIO 13  // Pino do fim de curso de inicio

// Parâmetros
// Movimento Linear
const int PASSOS_REVOLUCAO = 200; // Passos por volta do motor de passo
const int PULSOS_REVOLUCAO = 1600; // Pulsos por volta do motor de passo
const int PULSOS_PASSO = 1600 / 200; // Pulsos por passo do motor de passo
const int PASSO_FUSO = 8; // Passo do fuso linear por volta (em mm)
// Controle
extern int rpmPasso; // RPM do motor de passo
extern bool direcao = FRENTE; // Direção inicial do motor de passo
extern bool salvo = false; // Estado de configuração do indutor

// Funções
void offset();
void zerarMotorPasso();
void bobinar();


#endif