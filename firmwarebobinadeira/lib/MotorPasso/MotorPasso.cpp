#include "MotorPasso.h"
#include <Telas.h>
#include <MotorDC.h>

bool direcao = FRENTE; // Direção inicial do motor de passo
bool salvo = false; // Estado de configuração do indutor

// Função de offset do motor;
void offset() {
  const unsigned long DURACAO_PULSOS = (1.0 / ((240 / 60) * 1600) * 1000000) / 2; // Cálculo da duração dos pulsos

  int pulsos = 0;
  int passos = 0;
  int passosTotais = 1.0 * offsetPasso / (1.0 * PASSO_FUSO / PASSOS_REVOLUCAO);

  digitalWrite(DIRECAO_PASSO, FRENTE);
  digitalWrite(ENABLE, LOW);

  while (passos < passosTotais) {
    digitalWrite(PWM_PASSO, HIGH);
    delayMicroseconds(DURACAO_PULSOS);
    digitalWrite(PWM_PASSO, LOW);
    delayMicroseconds(DURACAO_PULSOS); 
    pulsos++; // Incrementa a quantidade de pulsos

    if (pulsos == PULSOS_PASSO) {
      pulsos = 0;
      passos++;
    }
  }

  digitalWrite(ENABLE, HIGH);
  delay(1000);
}

// Função para calibrar o motor (zero máquina)
void zerarMotorPasso() {
  const unsigned long duracaoPulso = (1.0 / ((240 / 60) * 1600) * 1000000) / 2; // Cálculo da duração dos pulsos

  // Configuração do motor
  digitalWrite(DIRECAO_PASSO, TRAS); // Define o sentido em direção ao zero
  digitalWrite(ENABLE, LOW); // Habilita o motor de passo

  telaAtual = telaCalibrarPasso();
  // Atua o motor de passo até o fim de curso
  while (digitalRead(INICIO)) {
    digitalWrite(PWM_PASSO, HIGH);
    delayMicroseconds(duracaoPulso);
    digitalWrite(PWM_PASSO, LOW);
    delayMicroseconds(duracaoPulso);
  }

  digitalWrite(ENABLE, HIGH); // Desabilita o motor de passo
  telaAtual = telaConcluirCalibragem();
  delay(2000);
}

// Função para rodar o motor
void bobinar() {
  salvo = true;
  // Retorna o motor ao zero máquina antes de iniciar o processo
  if (digitalRead(INICIO)) {
    zerarMotorPasso();
  }

  // Alinha o motor de passo com a ferramenta
  offset();

  // Incializa as variáveis de supervisão da rotina
  int pulsos = 0; 
  int passos = 0;
  int espiraAtual = 0;
  int espirasTotais = 0;
  int camadaAtual = 1;

  int espirasCamada = comprimento / diametro; // Calcula o número de espiras por camada
  
  // Calcula a quantidade de passos para cada espira do indutor
  const int PASSOS_ESPIRA = 1.0 * diametro / ((1.0 * PASSO_FUSO) / (1.0 * PASSOS_REVOLUCAO)); 

  // Calcula a duração dos pulsos com base no rpm
  const unsigned long DURACAO_PULSOS = (1.0 / ((1.0 * rpm / 60.0) * (1.0 * PASSOS_ESPIRA * PULSOS_PASSO))) * 1000000; // Duração dos pulsos em micro segundos

  digitalWrite(ENABLE, LOW); // Habilita o motor
  direcao = FRENTE;
  digitalWrite(DIRECAO_PASSO, direcao); // Configura a direção do motor

  telaAtual = telaProgresso(espirasTotais, camadaAtual);

  // Inicia o temporizador
  unsigned long ultimoPulso = 0;
  unsigned long ultimaAtualizacao = 0;

  // Atua o motor pelo número de voltas específicado
  ligarMotorDc();
  while (espirasTotais < espiras) {
    // Pulso PWM do motor de passo
    if (micros() - ultimoPulso >= DURACAO_PULSOS) {
      ultimoPulso = micros();

      digitalWrite(PWM_PASSO, HIGH);
      delayMicroseconds(1);
      digitalWrite(PWM_PASSO, LOW);

      // Incrementa a quantidade de pulsos
      pulsos++; 
    }

    // Verificação de progresso da rotina
    if (pulsos == PULSOS_PASSO) {
      pulsos = 0; // Reset da contagem de pulsos
      passos++; // Incrementação dos passos

      if (passos == PASSOS_ESPIRA) {
        passos = 0;
        espiraAtual++; // Incrementação de espiras da camada
        espirasTotais++; // Incrementação do total de espiras

        // Atualiza o progresso
        if (millis() - ultimaAtualizacao >= 1000) {
          atualizarEspira(espirasTotais);

          ultimaAtualizacao = millis();
        }

        // Verifica se o motor chegou ao fim do eixo linear ou ao fim da camada
        if (espiraAtual >= espirasCamada) {
          direcao = !direcao;
          camadaAtual++;
          espiraAtual = 0;

          digitalWrite(DIRECAO_PASSO, direcao);
          atualizarCamada(camadaAtual);
        }

        // Interrompe o processo ao bater no fim de curso
        if (!digitalRead(INICIO)) {
          // Serial.println("Processo interrompido: fim de curso acionado.");
          break;
        }

        // Interrompe o processo caso a porta seja aberta
        if (digitalRead(PIN_PORTA))
        {
          break;
        }
      }
    }
  }
  
  desligarMotorDc();
  digitalWrite(ENABLE, HIGH); // Desablita o motor de passo ao término da rotina

  // Feedback para o usuário
  atualizarEspira(espirasTotais);
  atualizarCamada(camadaAtual);
  delay(2000);
  telaAtual = telaInicial();
}