// Definição de pinagem
// Motor de passo
#define PWM_PASSO 5 // Pino PWM do motor de passo
#define DIRECAO_PASSO 4 // Pino de direção do motor de passo
#define ENABLE 2 // Pino enable do motor de passo

// Motor DC
#define TESTE_DC 6 // Pino para teste do acionamento do motor DC

// Fins de curso
#define FIM 12 // Pino do fim de curso 1
#define INICIO 13 // Pino do fim de curso 2

// Definição dos sentidos do motor
#define FRENTE false
#define TRAS true

// Parâmetros de movimento linear
const int PASSOS_REVOLUCAO = 200; // Quantidade de passos por volta do motor de passo
const int PULSOS_REVOLUCAO = 1600; // Quantidade de pulsos por volta do motor de passo
const int PULSOS_PASSO = PULSOS_REVOLUCAO / PASSOS_REVOLUCAO; // Quantidade de pulsos por passo do motor de passo
const int PASSO_FUSO = 8; // Passo do fuso por volta em mm

// Variáveis de controle do motor de passo
int rpmPasso; // RPM inicial do motor de passo
bool direcao = FRENTE; // Direção inicial do motor de passo

// Parâmetros do motor dc

int rpmDc = 320; // RPM inicial do motor DC

// Parâmetros do indutor
float comprimento; // Comprimento interno do indutor em mm
float diametro; // Bitola do fio em mm
int espiras; // Quantidade de espiras do indutor


void setup() {
  // Inicialização do Serial
  Serial.begin(9600);

  // Definição dos pinos dos fins de curso como entrada
  pinMode(FIM, INPUT);
  pinMode(INICIO, INPUT);

  // Acionamento do pull-up interno para os fins de curso
  digitalWrite(FIM, HIGH);
  digitalWrite(INICIO, HIGH);

  // Definição dos pinos do motor de passo como saída
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRECAO_PASSO, OUTPUT);
  pinMode(PWM_PASSO, OUTPUT);

  // Configuração inicial do motor de passo
  digitalWrite(ENABLE, HIGH); // Desabilita o motor
  digitalWrite(DIRECAO_PASSO, direcao); // Define a direção inicial do motor

  // Definição dos pinos do motor DC
  pinMode(TESTE_DC, OUTPUT);

  // Configuração inicial do motor DC
  digitalWrite(TESTE_DC, LOW);

  zerarMotorPasso(); // Calibra o motor de passo para o zero máquina
}

void loop() {
  // Teste dos fins de curso
  // if (!digitalRead(FIM) || !digitalRead(INICIO)) {
  //   Serial.print("Fim: ");
  //   Serial.println(digitalRead(FIM));
  //   Serial.print("Início: ");
  //   Serial.println(digitalRead(INICIO));
  //   delay(1000);
  // }

  // Leitura do comando inicial
  if (Serial.available() > 0) { // Verifica se existem dados para leitura na Serial
    String comando = Serial.readStringUntil('\n'); // Lê o comando

    // Rotina de controle do motor de passo
    if (comando == "r") {
      // Lê a quantidade de espiras do indutor
      Serial.print("Espiras: ");
      esperarInput();
      espiras = Serial.readStringUntil('\n').toInt();
      Serial.println(espiras);

      // Lê o comprimento do indutor
      Serial.print("Comprimento (mm): ");
      esperarInput();
      comprimento = Serial.readStringUntil('\n').toFloat();
      Serial.println(comprimento);

      // Lê o diâmetro do fio
      Serial.print("Bitola (mm): ");
      esperarInput();
      diametro = Serial.readStringUntil('\n').toFloat();
      Serial.println(diametro);

      Serial.println(""); // Quebra de linha

      // Inicia a rotina do motor
      bobinar();
    }
  }
}

// Funções de controle

// Função de offset do motor;
void offset() {
  const unsigned long DURACAO_PULSOS = (1.0 / ((240 / 60) * 1600) * 1000000) / 2; // Cálculo da duração dos pulsos

  int voltaAtual = 0;
  int pulsos = 0;
  int passos = 0;

  digitalWrite(DIRECAO_PASSO, FRENTE);
  digitalWrite(ENABLE, LOW);

  while (voltaAtual < 2) {
    digitalWrite(PWM_PASSO, HIGH);
    delayMicroseconds(DURACAO_PULSOS);
    digitalWrite(PWM_PASSO, LOW);
    delayMicroseconds(DURACAO_PULSOS); 
    pulsos++; // Incrementa a quantidade de pulsos

    if (pulsos == PULSOS_PASSO) {
      pulsos = 0;
      passos++;

      if (passos == PASSOS_REVOLUCAO) {
        passos = 0;
        voltaAtual++;
      }
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

  // Atua o motor de passo até o fim de curso
  while (digitalRead(INICIO)) {
    digitalWrite(PWM_PASSO, HIGH);
    delayMicroseconds(duracaoPulso);
    digitalWrite(PWM_PASSO, LOW);
    delayMicroseconds(duracaoPulso);
  }

  digitalWrite(ENABLE, HIGH); // Desabilita o motor de passo
  Serial.println("Calibrado!");
}

// Função para rodar o motor
void bobinar() {
  // Retorna o motor ao zero máquina antes de iniciar o processo
  if (digitalRead(INICIO)) {
    zerarMotorPasso();
    delay(1000);
  }

  offset();

  // Incializa as variáveis de supervisão da rotina
  bool primeiraVolta = true;
  int pulsos = 0; 
  int passos = 0;
  int espiraAtual = 0;
  int espirasTotais = 0;
  int camadaAtual = 1;

  int espirasCamada = comprimento / diametro; // Calcula o número de espiras por camada
  Serial.print("Espiras por camada: ");
  Serial.println(espirasCamada);
  
  int camadas = ceil(espiras / espirasCamada) + 1;
  Serial.print("Camadas: ");
  Serial.println(camadas);

  rpmPasso = (rpmDc * diametro) / PASSO_FUSO; // Calcula o RPM do motor de passo
  // Serial.print("RPM Passo: ");
  // Serial.println(rpmPasso);

  // Calcula a duração dos pulsos com base no rpm
  const unsigned long DURACAO_PULSOS = ((1.0 / ((rpmPasso / 60.0) * PULSOS_REVOLUCAO)) * 1000000); // Duração dos pulsos em micro segundos
  // Serial.print("Duração dos pulsos: ");
  // Serial.println(DURACAO_PULSOS);

  int passosEspira = diametro / (1.0 * PASSO_FUSO / PASSOS_REVOLUCAO); // Calcula a quantidade de passos para cada espira do indutor
  // Serial.print("Passos por espira: ");
  // Serial.println(passosEspira);

  float tempoEstimado = 1.0 * espiras * passosEspira / ((1.0 / (DURACAO_PULSOS / 1000000.0)) / PULSOS_PASSO); // Calcula o tempo estimado do processo

  // Exibe os parâmetros configurados e os parâmetros estimados (deslocamento e tempo)
  Serial.print("Tempo estimado: ");
  Serial.print(tempoEstimado);
  Serial.println(" segundos");

  Serial.print("\n"); // Quebra de linha

  digitalWrite(ENABLE, LOW); // Habilita o motor
  direcao = FRENTE;
  digitalWrite(DIRECAO_PASSO, direcao); // Configura a direção do motor

  unsigned long tempoAtual = millis(); // Inicia o temporizador
  unsigned long ultimoPulso = micros(); // Temporizador dos pulsos

  // Atua o motor pelo número de voltas específicado
  while (espirasTotais < espiras) {
    digitalWrite(TESTE_DC, HIGH); // Liga o motor DC

    // Pulso PWM do motor de passo
    if (micros() - ultimoPulso >= DURACAO_PULSOS) {
      ultimoPulso = micros();
      digitalWrite(PWM_PASSO, HIGH);
      delayMicroseconds(1);
      digitalWrite(PWM_PASSO, LOW);
      pulsos++; // Incrementa a quantidade de pulsos
    }

    // Verificação de progresso da rotina
    if (pulsos == PULSOS_PASSO) {
      pulsos = 0; // Reset da contagem de pulsos
      passos++; // Incrementação dos passos

      if (passos == passosEspira) {
        passos = 0;
        espiraAtual++; // Incrementação de espiras da camada
        espirasTotais++; // Incrementação do total de espiras

        // Exibição das voltas
        Serial.print(espirasTotais);
        Serial.print(" ");

        // Quebra a linha a cada 30 espiras
        if (espiraAtual % 30 == 0) {
          Serial.print("\n");
        }
      }
    }

    // Verifica se o motor chegou ao fim do eixo linear ou ao fim da camada
    if (espiraAtual == espirasCamada) {
      direcao = !direcao;

      camadaAtual++;
      espiraAtual = 0;

      digitalWrite(DIRECAO_PASSO, direcao);

      Serial.print("Camada: ");
      Serial.println(camadaAtual);
    }

    if (digitalRead(INICIO)) {
      primeiraVolta = false;
    }

    if (!digitalRead(FIM) || !digitalRead(INICIO)) {
      Serial.println("Processo interrompido: fim de curso acionado.");
      break;
    }
  }

  float tempoTotal = (millis() - tempoAtual) / 1000.0;

  digitalWrite(TESTE_DC, LOW); // Desliga o motor DC
  digitalWrite(ENABLE, HIGH); // Desablita o motor de passo ao término da rotina

  // Feedback para o usuário
  Serial.println("\n"); // Quebra de linha
  Serial.println("Pronto!");
  Serial.print("Tempo total: ");
  Serial.print(tempoTotal); // Calcula e exibe o tempo real de execução da rotina
  Serial.println(" segundos");
}




// Função para esperar entrada do usuário
void esperarInput() {
  while (Serial.available() == 0) {
    
  }
}