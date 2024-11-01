#include "MotorDC.h"

int rpm = 350;
volatile int pulsosEncoder = 0;
volatile bool ultimoEstadoClk = false;
unsigned long ultimoDebounce = 0;

// Função para ler os pulsos do encoder
void lerEncoder() {
  // Inicialização do temporizador
  unsigned long tempoAtual = millis();

  // Leitura do estado atual do encoder
  bool estadoClk = digitalRead(PIN_CLK);

  // Validação da mudação de estado e tempo de leitura do encoder
  if ((tempoAtual - ultimoDebounce) > DEBOUNCE && estadoClk != ultimoEstadoClk) {
    if (estadoClk == HIGH) {
      // Incrementação dos pulsos
      pulsosEncoder++;
    }

    ultimoDebounce = tempoAtual;
  }

  ultimoEstadoClk = estadoClk;
}

// Função de controle para acionar o motor DC
void ligarMotorDc() {
  digitalWrite(PIN_MOTOR_DC, HIGH);
}

// Função de controle para desligar o motor DC
void desligarMotorDc() {
  digitalWrite(PIN_MOTOR_DC, LOW);
}


// Função para medir o RPM do motor DC (para fins de calibragem)
void medirRPMDC() {
  // Zera a contgem de pulsos do encoder
  pulsosEncoder = 0;

  // Temporizador para o tempo de execução da função
  unsigned long tempo = millis();
  unsigned long tempoAtual = millis();
  int contagem = 5;

  // Acionamento do motor
  ligarMotorDc();

  // Habilita as interrupções de hardware para ler o encoder durante 5 segundos
  telaAtual = telaCalibrarRPM(contagem);
  attachInterrupt(digitalPinToInterrupt(PIN_CLK), lerEncoder, CHANGE);
  while (millis() - tempo < 5000) {

    if (millis() - tempoAtual >= 1000) {
      contagem--;
      tempoAtual = millis();
      telaAtual = telaCalibrarRPM(contagem);
    }
  }
  detachInterrupt(digitalPinToInterrupt(PIN_CLK));

  // Desacionamento do motor dc
  desligarMotorDc();

  // Cálculo do RPM
  rpm = ((pulsosEncoder / 20.0 / 5.0) * 60.0);

  Serial.print("RPM medido: ");
  Serial.println(rpm);
  telaAtual = telaConfirmarRPM(rpm);
  delay(2000);
}