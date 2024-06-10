#define PWM_PASSO 5 // Pino PWM do motor de passo
#define DIR_PASSO 4 // Pino de direção do motor de passo
#define ENABLE 2 // Pino enable do motor de passo

const int PASSOS_REVOLUCAO = 200; // Quantidade de passos por volta do motor de passo
const int PULSOS_REVOLUCAO = 1600; // Quantidade de pulsos por volta do motor de passo
const int PULSOS_PASSO = PULSOS_REVOLUCAO / PASSOS_REVOLUCAO; // Quantidade de pulsos por passo do motor de passo
const int PASSO_FUSO = 8; // Passo do fuso por volta em mm

// Variáveis de controle do motor de passo
int voltas = 0; // Voltas iniciais
int rpm = 120; // RPM inicial do motor de passo
bool dir = false; // Direção inicial do motor de passo

void setup() {
  // Inicialização do Serial
  Serial.begin(9600);

  // Definição de modo dos pinos do motor de passo
  pinMode(ENABLE, OUTPUT);
  pinMode(DIR_PASSO, OUTPUT);
  pinMode(PWM_PASSO, OUTPUT);

  // Configuração inicial do motor de passo
  digitalWrite(ENABLE, HIGH); // Desabilita o motor
  digitalWrite(DIR_PASSO, dir); // Define a direção inicial do motor
}

void loop() {
  // Leitura do comando inicial
  if (Serial.available() > 0) { // Verifica se existem dados para leitura na Serial
    String comando = Serial.readStringUntil('\n'); // Lê o comando

    // Rotina de controle do motor de passo
    if (comando == "r") {
      // Lê o número de voltas desejadas
      Serial.print("Voltas: ");
      esperarInput();
      voltas = Serial.readStringUntil('\n').toInt();
      Serial.println(voltas);

      // Lê a direção do motor
      Serial.print("Direção (0 - Frente | 1 - Trás): ");
      esperarInput();
      if (Serial.readStringUntil('\n').toInt() == 0) {
        dir = false;
        Serial.println("Frente");
      } else {
        dir = true;
        Serial.println("Trás");
      }

      // Lê o rpm desejado
      Serial.print("RPM: ");
      esperarInput();
      rpm = Serial.readStringUntil('\n').toInt();
      Serial.println(rpm);

      // Inicia a rotina do motor
      rodarPasso(voltas, rpm, dir);
    }
  }
}

// Funções de controle

// Função para rodar o motor
void rodarPasso(int voltasAlvo, int rpmAlvo, bool dir) {
  float tempoEstimado = voltasAlvo / (rpmAlvo / 60); // Calcula o tempo estimado do processo
  float deslocamentoEstimado = PASSO_FUSO * voltasAlvo; // Calucla o deslocamento estimado

  // Exibe os parâmetros configurados e os parâmetros estimados (deslocamento e tempo)
  Serial.print("\nRodando ");
  Serial.print(voltasAlvo);
  Serial.print(" voltas");
  Serial.print(" a ");
  Serial.print(rpmAlvo);
  Serial.println("RPM");
  Serial.print("Tempo estimado: ");
  Serial.print(tempoEstimado);
  Serial.println(" segundos");
  Serial.print("Deslocamento estimado: ");
  Serial.print(deslocamentoEstimado);
  Serial.println("mm\n");

  digitalWrite(ENABLE, LOW); // Habilita o motor
  digitalWrite(DIR_PASSO, dir); // Configura a direção do motor

  // Calcula a duração dos pulsos com base no rpm
  const unsigned long duracao_pulsos = ((1.0 / ((rpmAlvo / 60.0) * PULSOS_REVOLUCAO)) * 1000000) / 2; // Duração dos pulsos em micro segundos

  // Incializa as variáveis de supervisão da rotina
  int pulsos = 0; 
  int passos = 0;
  int voltaAtual = 0;

  unsigned long tempoAtual = millis(); // Inicia o temporizador

  // Atua o motor pelo número de voltas específicado
  while (voltaAtual < voltasAlvo) {
    // Pulso PWM do motor de passo
    digitalWrite(PWM_PASSO, HIGH);
    delayMicroseconds(duracao_pulsos);
    digitalWrite(PWM_PASSO, LOW);
    delayMicroseconds(duracao_pulsos); 
    pulsos++; // Incrementa a quantidade de pulsos

    // Verificação de progresso da rotina
    if (pulsos == PULSOS_PASSO) {
      pulsos = 0; // Reset da contagem de pulsos
      passos++; // Incrementação dos passos

      if (passos == PASSOS_REVOLUCAO) {
        passos = 0; // Reset dos passos
        voltaAtual++; // Incrementação da volta

        // Exibição das voltas
        Serial.print(voltaAtual);
        Serial.print(" ");
      }
    }
  }

  digitalWrite(ENABLE, HIGH); // Desablita o motor de passo ao término da rotina

  // Feedback para o usuário
  Serial.println("\n"); // Quebra de linha
  Serial.println("Pronto!");
  Serial.print("Tempo total: ");
  Serial.print((millis() - tempoAtual) / 1000); // Calcula e exibe o tempo real de execução da rotina
  Serial.println(" segundos");
}

// Função para esperar entrada do usuário
void esperarInput() {
  while (Serial.available() == 0) {

  }
}