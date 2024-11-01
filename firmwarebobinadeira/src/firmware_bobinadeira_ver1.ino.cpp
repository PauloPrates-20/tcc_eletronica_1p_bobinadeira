// Bibliotecas
#include <Keypad.h>
#include <Rele.h>
#include <Telas.h>

// Definições de hardware
// Motor DC
#define PIN_MOTOR_DC 6

// Encoder
#define PIN_CLK 18

// Motor de passo
#define PWM_PASSO 5      // Pino PWM do motor de passo
#define DIRECAO_PASSO 4  // Pino de direção do motor de passo
#define ENABLE 2         // Pino enable do motor de passo

// Definição dos sentidos do motor
#define FRENTE false
#define TRAS true

// Fins de curso
#define PORTA 12     // Pino do sensor de porta
#define INICIO 13  // Pino do fim de curso 2

// Keypad
#define LINHAS_TECLADO 4
#define COLUNAS_TECLADO 4

/* Variáveis */
int telaAtual = 1;
char quadrado = 255;

// Keypad
byte pinLinhasTeclado[LINHAS_TECLADO] = { 52, 50, 48, 46 };
byte pinColunasTeclado[COLUNAS_TECLADO] = { 44, 42, 40, 38 };

char teclas[LINHAS_TECLADO][COLUNAS_TECLADO] = {
  '1', '2', '3', 'A',
  '4', '5', '6', 'B',
  '7', '8', '9', 'C',
  '*', '0', '#', 'D'
};

// Instanciamento do keypad
Keypad teclado = Keypad(makeKeymap(teclas), pinLinhasTeclado, pinColunasTeclado, LINHAS_TECLADO, COLUNAS_TECLADO);

// Motor DC
// RPM inicial do motor DC
int rpm = 350;

// Encoder
// Contagem de pulsos
volatile int pulsosEncoder = 0;

// Último estado do encoder
volatile bool ultimoEstadoClk = LOW;

// Temporizador de debounce (para evitar ruídos na leitura) (em milissegundos)
unsigned long ultimoDebounce = 0;
const unsigned long DEBOUNCE = 0;

// Motor de passo
// Parâmetros de movimento linear
const int PASSOS_REVOLUCAO = 200;                              // Quantidade de passos por volta do motor de passo
const int PULSOS_REVOLUCAO = 1600;                             // Quantidade de pulsos por volta do motor de passo
const int PULSOS_PASSO = PULSOS_REVOLUCAO / PASSOS_REVOLUCAO;  // Quantidade de pulsos por passo do motor de passo
const int PASSO_FUSO = 8;                                      // Passo do fuso por volta em mm

// Variáveis de controle do motor de passo
int rpmPasso;           // RPM inicial do motor de passo
bool direcao = FRENTE;  // Direção inicial do motor de passo

// Estado da calibragem
bool calibrado = false;
bool salvo = false;  // Estado do indutor

void setup() {
  // Serial
  // Inicialização da comunicação Serial
  // Serial.begin(9600);

  // Motor DC
  // Inicialização do motor DC
  pinMode(PIN_MOTOR_DC, OUTPUT);
  digitalWrite(PIN_MOTOR_DC, LOW);

  pinMode(PIN_CLK, INPUT_PULLUP);

  // Fins de curso
  // Definição dos pinos dos fins de curso como entrada
  pinMode(PORTA, INPUT_PULLUP);
  pinMode(INICIO, INPUT_PULLUP);

  // Acionamento do pull-up interno para os fins de curso
  digitalWrite(PORTA, HIGH);
  digitalWrite(INICIO, HIGH);

  // Motor de passo
  // Definição dos pinos do motor de passo como saída
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRECAO_PASSO, OUTPUT);
  pinMode(PWM_PASSO, OUTPUT);

  // Configuração inicial do motor de passo
  digitalWrite(ENABLE, HIGH);            // Desabilita o motor
  digitalWrite(DIRECAO_PASSO, direcao);  // Define a direção inicial do motor

  // Módulo Relé
  // Configuração inicial
  pinMode(VERDE, OUTPUT);
  pinMode(AMARELO, OUTPUT);
  pinMode(VERMELHO, OUTPUT);
  digitalWrite(VERDE, HIGH);
  digitalWrite(AMARELO, HIGH);
  digitalWrite(VERMELHO, HIGH);

  // Interface
  // Inicialização da interface
  display.begin();
  display.backlight();

  telaAtual = telaAvisoCalibragem();
}

void loop() {
  // Teste fim de curso
  // if (!digitalRead(PORTA) || !digitalRead(INICIO)) {
  //   Serial.print("PORTA: ");
  //   Serial.println(digitalRead(PORTA));
  //   Serial.print("Início: ");
  //   Serial.println(digitalRead(INICIO));
  //   delay(300);
  // }

  // Interrupção de porta aberta
  while (digitalRead(PORTA)) {
    if (telaAtual != ERRO_PORTA) {
      telaAtual = telaErroPorta();
    }
  }

  if (telaAtual == ERRO_PORTA) {
    if (calibrado) {
      telaAtual = telaInicial();
    } else {
      telaAtual = telaAvisoCalibragem();
    }
  }

  // Controle IHM
  char tecla = teclado.getKey();
  // if (tecla) {
  //   Serial.println(tecla);
  //   Serial.println(telaAtual);
  // }

  switch (telaAtual) {
    case INICIAL:
      switch (tecla) {
        case '1':
          telaAtual = telaBobinar();
          break;
        case '2':
          telaAtual = telaAvisoCalibragem();
          break;
        case '0':
          parametro = "Offset";
          telaAtual = telaOffset(parametro, refOffset);
      }
      break;
    case BOBINAR:
      switch (tecla) {
        case '1':
          salvo = false;
          telaAtual = telaIndutor();
          break;
        case '2':
          if (salvo) {
            bobinar();
            break;
            case '#':
              telaAtual = telaInicial();
              break;
          }
          break;
      }
      break;
    case INDUTOR:
      switch (tecla) {
        case 'A':
          parametro = "Espiras";
          telaAtual = telaParametro(parametro, refEspiras);
          break;
        case 'B':
          parametro = "Comprimento";
          telaAtual = telaParametro(parametro, refComprimento);
          break;
        case 'C':
          parametro = "Diametro";
          telaAtual = telaParametro(parametro, refDiametro);
          break;
        case '*':
          if (refEspiras != "" && refComprimento != "" && refDiametro != "") telaAtual = telaConfirmarParametros();
          break;
        case '#':
          telaAtual = telaBobinar();
          break;
      }
      break;
    case PARAMETROS:
      switch (tecla) {
        case '*':
          if (parametro == "Espiras") {
            espiras = valorFormatado.toInt();
          } else if (parametro == "Comprimento") {
            comprimento = valorFormatado.toInt();
          } else if (parametro == "Diametro") {
            diametro = valorFormatado.toFloat();
          } else {
            offsetPasso = valorFormatado.toInt();
          }
          telaAtual = telaIndutor();
          break;
        case '#':
          if (parametro == "Espiras") {
            refEspiras.remove(refEspiras.length() - 1);
            telaAtual = telaParametro(parametro, refEspiras);
          } else if (parametro == "Comprimento") {
            refComprimento.remove(refComprimento.length() - 1);
            telaAtual = telaParametro(parametro, refComprimento);
          } else if (parametro == "Diametro") {
            refDiametro.remove(refDiametro.length() - 1);
            telaAtual = telaParametro(parametro, refDiametro);
          } else {
            refOffset.remove(refDiametro.length() - 1);
            telaAtual = telaParametro(parametro, refOffset);
          }
          break;
        default:
          if (isDigit(tecla)) {
            if (parametro == "Espiras") {
              if ((tecla == '0' && refEspiras.length() == 0) || refEspiras.length() > 3) {
                break;
              }
              refEspiras += tecla;
              telaAtual = telaParametro(parametro, refEspiras);
            } else if (parametro == "Comprimento") {
              if ((tecla == 0 && refComprimento.length() == 0) || refComprimento.length() > 2) {
                break;
              }
              refComprimento += tecla;
              telaAtual = telaParametro(parametro, refComprimento);
            } else if (parametro == "Diametro") {
              if ((tecla == '0' && refDiametro.length() == 0) || refDiametro.length() > 3) {
                break;
              }
              refDiametro += tecla;
              telaAtual = telaParametro(parametro, refDiametro);
            } else {
              if ((tecla == '0' && refOffset.length() == 0) || refDiametro.length() > 1) {
                break;
              }
              refOffset += tecla;
              telaAtual = telaParametro(parametro, refOffset);
            }
          }
          break;
      }
      break;
    case CONFIRMAR_PARAMETROS:
      switch (tecla) {
        case '*':
          salvo = true;
          refEspiras = "";
          refComprimento = "";
          refDiametro = "";

          bobinar();
          break;
        case '#':
          telaAtual = telaIndutor();
          break;
      }
      break;
    case OFFSET:
      switch (tecla) {
        case '*':
          if (refOffset != "") {
            offsetPasso = valorFormatado.toInt();
            telaAtual = telaInicial();
          }
          break;
        case '#':
          refOffset.remove(refDiametro.length() - 1);
          telaAtual = telaOffset(parametro, refOffset);
          break;
        default:
          if (isDigit(tecla)) {
            if ((tecla == '0' && refOffset.length() == 0) || refDiametro.length() > 1) {
              break;
            }
            refOffset += tecla;
            telaAtual = telaOffset(parametro, refOffset);
          }
          break;
      }
      break;
    case AVISO_CALIBRAGEM:
      switch (tecla) {
        case '*':
          calibrar();
          break;
        case '#':
          telaAtual = telaInicial();
          break;
      }
      break;
  }
}

// Função para calibrar os motores
void calibrar() {
  Serial.println("Calibrando...");
  medirRPMDC();

  while (rpm == 0) {
    telaAtual = telaErroCalibragem();
    delay(2000);
    medirRPMDC();
  }

  zerarMotorPasso();
  Serial.println("Calibrado");
  telaAtual = telaInicial();
  calibrado = true;
}

// Função para esperar entrada do usuário
void esperarInput() {
  while (Serial.available() == 0) {
  }
}