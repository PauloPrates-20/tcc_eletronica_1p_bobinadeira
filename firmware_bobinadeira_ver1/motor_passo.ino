// Função de offset do motor;
void offset() {
  const unsigned long DURACAO_PULSOS = (1.0 / ((240 / 60) * 1600) * 1000000) / 2; // Cálculo da duração dos pulsos

  int pulsos = 0;
  int passos = 0;
  float deslocamento = 15.00;
  int passosTotais = deslocamento / (1.0 * PASSO_FUSO / PASSOS_REVOLUCAO);

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

  telaAtual = calibrarPasso();
  // Atua o motor de passo até o fim de curso
  while (digitalRead(INICIO)) {
    digitalWrite(PWM_PASSO, HIGH);
    delayMicroseconds(duracaoPulso);
    digitalWrite(PWM_PASSO, LOW);
    delayMicroseconds(duracaoPulso);
  }

  digitalWrite(ENABLE, HIGH); // Desabilita o motor de passo
  telaAtual = concluirCalibragem();
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

  // Serial.println(espiras);
  // Serial.println(comprimento);
  // Serial.println(diametro);

  int espirasCamada = comprimento / diametro; // Calcula o número de espiras por camada
  // Serial.print("Espiras por camada: ");
  // Serial.println(espirasCamada);
  
  int camadas = ceil(espiras / espirasCamada) + 1;
  // Serial.print("Camadas: ");
  // Serial.println(camadas);

  // Calcula a quantidade de passos para cada espira do indutor
  const int PASSOS_ESPIRA = 1.0 * diametro / ((1.0 * PASSO_FUSO) / (1.0 * PASSOS_REVOLUCAO)); 
  // Serial.print("Passos por espira: ");
  // Serial.println(PASSOS_ESPIRA);

  // Calcula a duração dos pulsos com base no rpm
  const unsigned long DURACAO_PULSOS = (1.0 / ((1.0 * rpm / 60.0) * (1.0 * PASSOS_ESPIRA * PULSOS_PASSO))) * 1000000; // Duração dos pulsos em micro segundos
  // Serial.print("Duração dos pulsos: ");
  // Serial.println(DURACAO_PULSOS);

  // Calcula o tempo estimado do processo
  float tempoEstimado = espiras * 1.0 / (rpm / 60.0);

  // Exibe os parâmetros configurados e os parâmetros estimados (deslocamento e tempo)
  Serial.print("Tempo estimado: ");
  Serial.print(tempoEstimado);
  Serial.println(" segundos");

  digitalWrite(ENABLE, LOW); // Habilita o motor
  direcao = FRENTE;
  digitalWrite(DIRECAO_PASSO, direcao); // Configura a direção do motor

  telaAtual = telaProgresso(espirasTotais, camadaAtual);

  // Inicia o temporizador
  unsigned long tempoAtual = micros();
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

        // Exibição das voltas
        // Serial.print(espirasTotais);
        // Serial.print(" ");

        // Quebra a linha a cada 30 espiras
        // if (espiraAtual % 30 == 0) {
        //   Serial.print("\n");
        // }

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

          // Serial.print("\nCamada: ");
          // Serial.println(camadaAtual);
        }

        if (!digitalRead(FIM) || !digitalRead(INICIO)) {
          // Serial.println("Processo interrompido: fim de curso acionado.");
          break;
        }
      }
    }
  }
  float tempoTotal = (micros() - tempoAtual) / 1000000.0;

  desligarMotorDc();
  digitalWrite(ENABLE, HIGH); // Desablita o motor de passo ao término da rotina

  // Feedback para o usuário
  Serial.println("Pronto!");
  Serial.print("Tempo total: ");
  Serial.print(tempoTotal); // Calcula e exibe o tempo real de execução da rotina
  Serial.println(" segundos\n");
  atualizarEspira(espirasTotais);
  atualizarCamada(camadaAtual);
  delay(2000);
  telaAtual = telaInicial();
}