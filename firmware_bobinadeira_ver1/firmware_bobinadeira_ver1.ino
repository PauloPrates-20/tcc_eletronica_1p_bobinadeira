/* Bibliotecas */
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

/* Definição de hardware */

// Motor DC
#define PIN_MOTOR_DC 6
#define PIN_CLK 18

// Motor de passo
#define PWM_PASSO 5      // Pino PWM do motor de passo
#define DIRECAO_PASSO 4  // Pino de direção do motor de passo
#define ENABLE 2         // Pino enable do motor de passo

// Definição dos sentidos do motor
#define FRENTE false
#define TRAS true

// Fins de curso
#define FIM 12     // Pino do fim de curso 1
#define INICIO 13  // Pino do fim de curso 2

// Display
#define ENDERECO_DISPLAY 0x38
#define COLUNAS_DISPLAY 20
#define LINHAS_DISPLAY 4

// Keypad
#define LINHAS_TECLADO 4
#define COLUNAS_TECLADO 4

/* Variáveis */

// Display
/* 
  | Controle de telas
  | -----------------------------
  | id -1 = erro de comando
  | id 1 = Tela de Seleção
  | id 2x = Telas de execução
  | id 3x = Telas de Calibragem
*/
int telaAtual = 1;
char quadrado = 255;

/* 
  | Instanciamento do display
  | -------------------------------------
  | endereço 0x27 no modelo físico
  | endereço 0x38 para teste no proteus
*/
LiquidCrystal_I2C display(0x27, COLUNAS_DISPLAY, LINHAS_DISPLAY);

// Keypad
byte pinLinhasTeclado[LINHAS_TECLADO] = { 2, 3, 4, 5 };
byte pinColunasTeclado[COLUNAS_TECLADO] = { 6, 7, 8, 9 };

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

/* Encoder */
// Contagem de pulsos
volatile int pulsosEncoder = 0;

// Último estado do encoder
volatile bool ultimoEstadoClk = LOW;

// Temporizador de debounce (para evitar ruídos na leitura) (em milissegundos)
unsigned long ultimoDebounce = 0;
const unsigned long DEBOUNCE = 2;

/* Motor de passo */
// Parâmetros de movimento linear
const int PASSOS_REVOLUCAO = 200;                              // Quantidade de passos por volta do motor de passo
const int PULSOS_REVOLUCAO = 1600;                             // Quantidade de pulsos por volta do motor de passo
const int PULSOS_PASSO = PULSOS_REVOLUCAO / PASSOS_REVOLUCAO;  // Quantidade de pulsos por passo do motor de passo
const int PASSO_FUSO = 8;                                      // Passo do fuso por volta em mm

// Variáveis de controle do motor de passo
int rpmPasso;           // RPM inicial do motor de passo
bool direcao = FRENTE;  // Direção inicial do motor de passo

// Parâmetros do indutor
String refEspiras = "";      // Espiras do indutor na IHM
String refComprimento = "";  // Comprimento do indutor na IHM
String refDiametro = "";     // Diametro do filamento na IHM
String parametro;            // Parametro a exibir no display
String valorFormatado;       // Valor do parametro formatado para exibição

int espiras;         // Quantidade de espiras do indutor
int comprimento;     // Comprimento interno do indutor em mm
float diametro;      // Bitola do fio em mm
bool salvo = false;  // Estado do indutor

/* Protótipos de funções */

// Display
// Funções auxiliares
int centralizarDisplay(String texto);
void atualizarAndamento(int espiras, int camadas);
void formatarValor(String parametro, String valor);

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
  pinMode(FIM, INPUT_PULLUP);
  pinMode(INICIO, INPUT_PULLUP);

  // Acionamento do pull-up interno para os fins de curso
  digitalWrite(FIM, HIGH);
  digitalWrite(INICIO, HIGH);

  /* Motor de Passo */
  // Definição dos pinos do motor de passo como saída
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRECAO_PASSO, OUTPUT);
  pinMode(PWM_PASSO, OUTPUT);

  // Configuração inicial do motor de passo
  digitalWrite(ENABLE, HIGH);            // Desabilita o motor
  digitalWrite(DIRECAO_PASSO, direcao);  // Define a direção inicial do motor

  /* Interface */
  // Inicialização da interface
  display.begin();
  display.backlight();

  calibrar();
}

void loop() {
  /* Teste dos fins de curso */
  // if (!digitalRead(FIM) || !digitalRead(INICIO)) {
  //   Serial.print("Fim: ");
  //   Serial.println(digitalRead(FIM));
  //   Serial.print("Início: ");
  //   Serial.println(digitalRead(INICIO));
  //   delay(300);
  // }

  /* Controle IHM */
  char tecla = teclado.getKey();
  if (tecla) {
    Serial.println(tecla);
    Serial.println(telaAtual);
  }

  switch (telaAtual) {
    case 1:
      switch (tecla) {
        case '1':
          telaAtual = telaBobinar();
          break;
        case '2':
          telaAtual = telaAvisoCalibragem();
          break;
      }
      break;
    case 20:
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
    case 21:
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
    case 22:
      switch (tecla) {
        case '*':
          if (parametro == "Espiras") {
            espiras = valorFormatado.toInt();
          } else if (parametro == "Comprimento") {
            comprimento = valorFormatado.toInt();
          } else {
            diametro = valorFormatado.toFloat();
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
          } else {
            refDiametro.remove(refDiametro.length() - 1);
            telaAtual = telaParametro(parametro, refDiametro);
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
            } else {
              if ((tecla == '0' && refDiametro.length() == 0) || refDiametro.length() > 3) {
                break;
              }
              refDiametro += tecla;
              telaAtual = telaParametro(parametro, refDiametro);
            }
          }
          break;
      }
      break;
    case 23:
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
    case 30:
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

/* Controle Serial */
// Leitura do comando inicial
// if (Serial.available() > 0) { // Verifica se existem dados para leitura na Serial
//   String comando = Serial.readStringUntil('\n'); // Lê o comando

//   // Rotina de controle do motor de passo
//   if (comando == "r") {
//     salvo = false;
//     // Lê a quantidade de espiras do indutor
//     Serial.print("Espiras: ");
//     esperarInput();
//     espiras = Serial.readStringUntil('\n').toInt();
//     Serial.println(espiras);

//     // Lê o comprimento do indutor
//     Serial.print("Comprimento (mm): ");
//     esperarInput();
//     comprimento = Serial.readStringUntil('\n').toFloat();
//     Serial.println(comprimento);

//     // Lê o diâmetro do fio
//     Serial.print("Bitola (mm): ");
//     esperarInput();
//     diametro = Serial.readStringUntil('\n').toFloat();
//     Serial.println(diametro);

//     Serial.println(""); // Quebra de linha

//     // Inicia a rotina do motor
//     bobinar();
//   } else if (comando == "u" && salvo) {
//     bobinar();
//   } else if (comando == "c") {
//     Serial.println("Calibrando...");
//     calibrar();
//     Serial.println("Motores calibrados.");
//   }
// }
}

// Função para calibrar os motores
void calibrar() {
  Serial.println("Calibrando...");
  medirRPMDC();
  zerarMotorPasso();
  Serial.println("Calibrado");
  telaAtual = telaInicial();
}

// Função para esperar entrada do usuário
void esperarInput() {
  while (Serial.available() == 0) {
  }
}