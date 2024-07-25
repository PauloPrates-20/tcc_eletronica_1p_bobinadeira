// Definição de pinagem

/* Motor DC */
#define PIN_MOTOR_DC 6
#define PIN_CLK 20

/* Motor de passo */
#define PWM_PASSO 5 // Pino PWM do motor de passo
#define DIRECAO_PASSO 4 // Pino de direção do motor de passo
#define ENABLE 2 // Pino enable do motor de passo

// Definição dos sentidos do motor
#define FRENTE false
#define TRAS true

/* Fins de curso */
#define FIM 12 // Pino do fim de curso 1
#define INICIO 13 // Pino do fim de curso 2

/* Variáveis */

/* Motor DC */
// RPM inicial do motor DC
int rpm = 350;

/* Encoder */
// Contagem de pulsos
volatile int pulsosEncoder = 0;

// Último estado do encoder
volatile bool ultimoEstadoClk = LOW;

// Temporizador de debounce (para evitar ruídos na leitura) (em milissegundos)
unsigned long ultimoDebounce = 0;
const unsigned long DEBOUNCE = 5;

/* Motor de passo */
// Parâmetros de movimento linear
const int PASSOS_REVOLUCAO = 200; // Quantidade de passos por volta do motor de passo
const int PULSOS_REVOLUCAO = 1600; // Quantidade de pulsos por volta do motor de passo
const int PULSOS_PASSO = PULSOS_REVOLUCAO / PASSOS_REVOLUCAO; // Quantidade de pulsos por passo do motor de passo
const int PASSO_FUSO = 8; // Passo do fuso por volta em mm

// Variáveis de controle do motor de passo
int rpmPasso; // RPM inicial do motor de passo
bool direcao = FRENTE; // Direção inicial do motor de passo

// Parâmetros do motor dc

// Parâmetros do indutor
float comprimento; // Comprimento interno do indutor em mm
float diametro; // Bitola do fio em mm
int espiras; // Quantidade de espiras do indutor]
bool salvo = false;

void setup() {
  /* Serial */
  // Inicialização da comunicação Serial
  Serial.begin(9600);

  /* Motor DC */
  // Inicialização do motor DC
  pinMode(PIN_MOTOR_DC, OUTPUT);
  digitalWrite(PIN_MOTOR_DC, LOW);

  pinMode(PIN_CLK, INPUT_PULLUP);

  /* Fins de curso */
  // Definição dos pinos dos fins de curso como entrada
  pinMode(FIM, INPUT);
  pinMode(INICIO, INPUT);

  // Acionamento do pull-up interno para os fins de curso
  digitalWrite(FIM, HIGH);
  digitalWrite(INICIO, HIGH);

  /* Motor de Passo */
  // Definição dos pinos do motor de passo como saída
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRECAO_PASSO, OUTPUT);
  pinMode(PWM_PASSO, OUTPUT);

  // Configuração inicial do motor de passo
  digitalWrite(ENABLE, HIGH); // Desabilita o motor
  digitalWrite(DIRECAO_PASSO, direcao); // Define a direção inicial do motor

  calibrar();
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
      salvo = false;
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
    } else if (comando == "u" && salvo) {
      bobinar();
    } else if (comando == "c") {
      Serial.println("Calibrando...");
      calibrar();
      Serial.println("Motores calibrados.");
    }
  }
}

// Função para calibrar os motores
void calibrar() {
  Serial.println("Calibrando...");
  medirRPMDC();
  zerarMotorPasso();
  Serial.println("Calibrado");
}

// Função para esperar entrada do usuário
void esperarInput() {
  while (Serial.available() == 0) {
    
  }
}